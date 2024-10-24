# CodeSandbox
Môi trường chấm mã nguồn tự động dùng cho các cuộc thi lập trình thi đấu, được xây dựng cho hệ điều hành Linux.

## Tính năng
- Hỗ trợ cấu hình nhiều ngôn ngữ thông qua abstract class.
- Giao tiếp thông qua HTTP API
- Hỗ trợ giới hạn tài nguyên (bộ nhớ, thời gian chạy) cho môi trường chấm.

## Cài đặt
- Build: ...
- Docker: ...

## Cấu hình máy chủ HTTP
Sử dụng các biến môi trường sau:
- `PORT` (mặc định: `4000`): Chỉ định cổng HTTP để máy chủ sử dụng.

## API
- `GET /version`: Trả về phiên bản của chương trình, có thể sử dụng để kiểm tra kết nối từ client.
> Phản hồi: `text/plain`

- `GET /problems`: Trả về danh sách các bài chấm đã được nhập vào chương trình.
> Phản hồi: `application/json`

- `GET /modules`: Trả về danh sách các module có thể lựa chọn để chấm ở chương trình.
> Phản hồi: `application/json`

- `POST /submit`:  Tạo yêu cầu chấm
> Yêu cầu: `multipart/form-data`
> - `id`(string): ID yêu cầu
> - `problem_id` (string): ID bài chấm
> - `target_module` (string): Module chấm sử dụng cho yêu cầu
> - `file` (file): Tệp mã nguồn

> Phản hồi: `application/json`
> - `id`(string): ID yêu cầu chấm
> - `status` (enum): Accepted, WrongAnswer, TimeLimitExceeded, MemoryLimitExceeded, RuntimeError, CompilationError, InternalError
> - `message` (string): Phản hồi từ trình chấm

## Cấu hình bài chấm
Mỗi bài chấm cần được đặt trong thư mục `problems/<problem_id>` so với thư mục chạy chương trình.
Ví dụ;
```txt
problems/
├── TEST1/
│   ├── testcases/
│   │   ├── 1.in
│   │   ├── 1.out
│   │   ├── 2.in
│   │   └── 2.out
│   └── config.cfg
├── TEST2/
│   ├── testcases/
│   │   ├── 1.in
│   │   ├── 1.out
│   │   ├── 2.in
│   │   └── 2.out
│   └── config.cfg
main/

```
> Tệp cấu hình: `config.cfg`
> ```cfg
> TEST_CASES=2
> MEMORY_LIMIT=512
> TIME_LIMIT=1
> ```
> - `TEST_CASES`: Số lượng test case của bài chấm, tương ứng với số cặp file `<number>.in` và `<number>.out` trong thư mục con `testcases/`. Cần có đủ số cặp file `<number>.in` và `<number>.out` trong cầu hình để chương trình có thể nạp bài chấm thành công.
> - `MEMORY_LIMIT` (không bắt buộc): Giới hạn dung lượng bộ nhớ (RAM) cho chương trình trong quá trình chấm, tính theo đơn vị MiB.
> - `TIME_LIMIT` (không bắt buộc): Giới hạn thời gian chạy của chương trình trong mỗi testcase, tính theo giây.

## Các dự án có thể sử dụng trình chấm này
