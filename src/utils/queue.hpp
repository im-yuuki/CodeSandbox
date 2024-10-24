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
        explicit ThreadSafeQueue(std::function<void(T)> task) : task(std::move(task)) {}

        // Add an element to the queue
        void enqueue(T item) {
            {
                std::unique_lock lock(mtx);
                queue.push(std::move(item));
                if (!thread_running) {
                    thread_running = true;
                    worker_thread = std::thread(&ThreadSafeQueue::processQueue, this);
                }
            }
            cv.notify_one(); // Notify the worker thread to process the new item
        }

        // Process the queue
        void processQueue() {
            while (true) {
                T item;  // Use item instead of pointer
                {
                    std::unique_lock lock(mtx);
                    cv.wait(lock, [this] { return !queue.empty(); });

                    item = std::move(queue.front()); // Move item out of queue
                    queue.pop();
                }

                try {
                    task(item);  // Call the task with the item
                } catch (const std::exception& e) {
                    logger->error("Error processing item: {}", e.what());
                }

                {
                    std::unique_lock lock(mtx);
                    // If the queue is empty, set thread_running to false and exit
                    if (queue.empty()) {
                        thread_running = false;
                        break; // Exit if the queue is empty
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

    inline void processSubmission(data::Submission submission) { // Change to pass by value
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
    }

    static ThreadSafeQueue<data::Submission> submission_queue(processSubmission);

}

#endif
