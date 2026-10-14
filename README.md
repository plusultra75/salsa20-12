# SALSA20/12 – STREAM CIPHER

## 1. Giới thiệu dự án

Dự án **“Nghiên cứu và cài đặt thuật toán Salsa20/12”** thuộc môn Lý thuyết mật mã, được thực hiện bởi nhóm 6 thành viên bằng ngôn ngữ C.

Salsa20/12 là thuật toán mã hóa dòng (*Stream Cipher*) sử dụng 12 vòng xử lý, kết hợp các phép cộng modulo \(2^{32}\), XOR và xoay bit.

**Mục tiêu:** Tìm hiểu nguyên lý hoạt động, cài đặt thuật toán và thực hành phát triển phần mềm theo nhóm thông qua GitHub.

## 2. Phân công công việc

| Thành viên | Module phụ trách |
|---|---|
| Thành viên 1 | Quarter Round và `rotl32()` |
| Thành viên 2 | Row Round |
| Thành viên 3 | Column Round |
| Thành viên 4 | Double Round và Salsa20 Core |
| Thành viên 5 | Khởi tạo State |
| Thành viên 6 | Mã hóa và giải mã |

Mỗi thành viên tự nghiên cứu, lập trình, kiểm thử và chịu trách nhiệm về module được phân công.

## 3. Cấu trúc dự án

- `include/`: Chứa các file header khai báo hàm dùng chung.
- `src/`: Chứa mã nguồn các module của nhóm.
- `test/`: Chứa các bài Unit Test.
- `.github/`: Chứa cấu hình quản lý và review mã nguồn.
- `.gitignore`: Loại trừ các file tạm không cần đưa lên GitHub.

Các module được phát triển trên những nhánh riêng và tích hợp dần vào nhánh `main` sau khi được review.

## 4. Quy trình làm việc nhóm

1. Mỗi thành viên làm việc trên một nhánh `feature/` riêng.
2. Commit theo từng giai đoạn hoàn thành công việc.
3. Kiểm tra mã nguồn trước khi gửi lên GitHub.
4. Tạo Pull Request để yêu cầu review.
5. Chỉ merge vào `main` sau khi đáp ứng các yêu cầu phê duyệt.

**Quy định:** Không tự ý thay đổi mã nguồn của thành viên khác hoặc đưa code trực tiếp vào nhánh `main`.

