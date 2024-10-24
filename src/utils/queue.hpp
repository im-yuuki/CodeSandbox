#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <vector>
#include <spdlog/logger.h>

#include "logging.hpp"
#include "../data/problems.hpp"
#include "../data/submissions.hpp"
#include "../data/storage.hpp"
#include "../modules/abstract.hpp"
#include "../modules/register.hpp"

namespace queue {

    static auto logger = logging::create_logger("queue");

    template <typename T>
    class ThreadSafeQueue {
    private:
        std::queue<T> queue;
        std::mutex mtx;
        std::condition_variable cv;
        std::thread worker_thread;
        std::function<void(T)> task;
        bool thread_running = false;

    public:
        explicit ThreadSafeQueue(std::function<void(T&)> task) : task(task) {}

        // Add an element to the queue and start a new thread if no thread is running
        void enqueue(T item) {
            bool start_thread = false;
            {
                std::unique_lock lock(mtx);
                queue.push(std::move(item));
                if (!thread_running) {
                    thread_running = true;
                    start_thread = true;
                }
            }
            cv.notify_one();
            if (start_thread) {
                worker_thread = std::thread(&ThreadSafeQueue::processQueue, this);
            }
        }

        // Process the queue
        void processQueue() {
            while (true) {
                T item;
                {
                    std::unique_lock lock(mtx);
                    cv.wait(lock, [this] { return !queue.empty(); });
                    item = std::move(queue.front());
                    queue.pop();
                }
                task(item);
                {
                    std::unique_lock lock(mtx);
                    if (queue.empty()) {
                        thread_running = false;
                        break;
                    }
                }
            }
        }

        // Check if the queue is empty
        bool empty() {
            std::unique_lock lock(mtx);
            return queue.empty();
        }

        // Join the worker thread
        void joinWorker() {
            if (worker_thread.joinable()) {
                worker_thread.join();
            }
        }
    };

    inline void processSubmission(data::Submission& submission) {
        logger->info("Start processing for submission: {}", submission.id);
        try {
            const data::Problem problem = data::get_problem(submission.problem_id);
            handlers::IModules* handler = handlers::create_handler(submission, problem);
            handler->run();
        } catch (const std::exception& e) {
            submission.status = data::submission_status::InternalError;
            submission.message = e.what();
        }
        data::result_list.emplace_back(submission.id, submission.status, submission.message);
        delete submission.file_content;
    }

    static ThreadSafeQueue<data::Submission> submission_queue(processSubmission);

}

#endif
