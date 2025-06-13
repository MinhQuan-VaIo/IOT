# 🌤️ WeatherApp

**WeatherApp** is a web application built with ASP.NET Core Razor Pages that allows users to retrieve weather information for any city using the OpenWeatherMap API. It features a modern, responsive UI and user-friendly error handling.

---

## 🚀 Key Features

- **Weather Search**: Enter a city name to fetch weather data such as temperature, humidity, wind speed, and description.
- **Responsive Design**: Built with Bootstrap 5.3 for compatibility across devices.
- **Error Handling**: Displays clear messages for invalid city names or API issues.
- **Logging**: Logs API responses and errors for easier debugging.

---

## 🧪 Technologies Used

- **Backend**: ASP.NET Core 6.0 (C#), Razor Pages
- **Frontend**: HTML, CSS, Bootstrap 5.3, JavaScript
- **API**: [OpenWeatherMap](https://openweathermap.org/)
- **Libraries**: `System.Text.Json`, `Microsoft.Extensions.Logging`

---

## ▶️ Running the Application

### 1. Build the Project
```bash
dotnet build
```

### 2. Run the Application
```bash
dotnet run
```

The application will be available at `http://localhost:5272` (or another port as shown in your terminal).

---

## 🧪 How to Use

### 🌍 Search:
- Enter a city name (e.g., `Hà Nội`, `Tokyo`) in the search box.
- Click **Search** to fetch the weather data.

### 📊 Displayed Results:
- City and country
- Temperature (°C)
- Weather description (e.g., "clear sky")
- Humidity (%)
- Wind speed (m/s)

### ⚠️ Error Handling:
- Displays a warning message if the city name is invalid or if the API fails.

---


## 📚 Author Info

- **Assignment**: Exercise 2 – API Integration and Dynamic Data Processing  
- **Course**: Internship (HK243)  
- **Language**: C# (ASP.NET Core)  
- **Purpose**: Educational use and assignment submission

---

## 📜 License

> This project was developed for educational purposes and does not carry any commercial or distribution license.
