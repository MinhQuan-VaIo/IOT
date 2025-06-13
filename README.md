# 🌤️ WeatherApp

**WeatherApp** là một ứng dụng web sử dụng ASP.NET Core Razor Pages, cho phép người dùng tra cứu thông tin thời tiết của bất kỳ thành phố nào thông qua API từ OpenWeatherMap. Giao diện hiện đại, dễ sử dụng và xử lý lỗi thân thiện.

---

## 🚀 Tính năng chính

- 🔍 **Tra cứu thời tiết**: Nhập tên thành phố để lấy dữ liệu thời tiết như nhiệt độ, độ ẩm, tốc độ gió và mô tả thời tiết.
- 📱 **Giao diện responsive**: Thiết kế bằng Bootstrap 5.3, hiển thị tốt trên cả máy tính và thiết bị di động.
- ⚠️ **Xử lý lỗi thân thiện**: Cảnh báo rõ ràng khi nhập sai tên thành phố hoặc có lỗi từ API.
- 🧾 **Ghi log**: Ghi lại phản hồi API và lỗi để dễ kiểm tra khi debug.

---

## 🧪 Công nghệ sử dụng

- **Backend**: ASP.NET Core 6.0 (C#), Razor Pages
- **Frontend**: HTML, CSS, Bootstrap 5.3, JavaScript
- **API**: [OpenWeatherMap](https://openweathermap.org/)
- **Thư viện**: `System.Text.Json`, `Microsoft.Extensions.Logging`

---

## 💻 Yêu cầu hệ thống

- .NET SDK **6.0 trở lên** – [Tải tại đây](https://dotnet.microsoft.com/download)
- IDE đề xuất: **Visual Studio Code** với extension C#
- API Key từ OpenWeatherMap – [Đăng ký miễn phí tại đây](https://openweathermap.org/)
- Hệ điều hành: Windows, macOS hoặc Linux

---


## ▶️ Chạy ứng dụng

### 1. Build dự án
```bash
dotnet build
```

### 2. Chạy ứng dụng
```bash
dotnet run
```

Ứng dụng sẽ chạy tại địa chỉ `http://localhost:5272` (hoặc cổng khác tùy hệ thống – xem terminal khi chạy).

---

## 🧪 Sử dụng

### 🌍 Tìm kiếm:
- Nhập tên thành phố (ví dụ: `Hà Nội`, `Tokyo`) vào ô tìm kiếm.
- Nhấn **Search** để nhận kết quả thời tiết.

### 📊 Kết quả hiển thị:
- Tên thành phố và quốc gia
- Nhiệt độ (°C)
- Mô tả thời tiết (ví dụ: "clear sky")
- Độ ẩm (%)
- Tốc độ gió (m/s)

### ⚠️ Xử lý lỗi:
- Hiển thị thông báo lỗi khi nhập sai thành phố hoặc API lỗi.

---

## 📚 Thông tin tác giả

- **Bài tập**: Exercise 2 – API Integration and Dynamic Data Processing  
- **Học phần**: Thực tập chuyên ngành – HK243  
- **Ngôn ngữ**: C# (ASP.NET Core)  
- **Mục đích**: Phục vụ học tập và nộp bài

---

## 📜 Giấy phép

> Dự án này được phát triển cho mục đích học tập, không có giấy phép thương mại hoặc phân phối.

---

## 📸 (Tuỳ chọn) Ảnh minh hoạ
> ![Demo Screenshot](./screenshot.png)
