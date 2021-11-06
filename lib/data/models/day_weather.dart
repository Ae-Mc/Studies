import 'package:json_annotation/json_annotation.dart';
import 'package:weather_app/data/models/weather.dart';

part 'day_weather.g.dart';

@JsonSerializable(createToJson: false)
class DayWeather {
  final List<WeatherData> weather;
  @JsonKey(fromJson: _dateTimeFromMillisecondsSinceEpoch)
  final DateTime dt;
  final Temp temp;
  final int pressure;
  final int humidity;
  @JsonKey(name: 'wind_speed')
  final double windSpeed;

  DayWeather({
    required this.weather,
    required this.dt,
    required this.temp,
    required this.pressure,
    required this.humidity,
    required this.windSpeed,
  });

  factory DayWeather.fromJson(Map<String, dynamic> json) =>
      _$DayWeatherFromJson(json);

  static DateTime _dateTimeFromMillisecondsSinceEpoch(int milliseconds) {
    return DateTime.fromMillisecondsSinceEpoch(milliseconds, isUtc: true);
  }
}

@JsonSerializable(createToJson: false)
class Temp {
  @JsonKey(name: 'morn')
  final double morning;
  final double day;
  @JsonKey(name: 'eve')
  final double evening;
  final double night;
  final double min;
  final double max;

  Temp({
    required this.morning,
    required this.day,
    required this.evening,
    required this.night,
    required this.min,
    required this.max,
  });

  factory Temp.fromJson(Map<String, dynamic> json) => _$TempFromJson(json);
}
