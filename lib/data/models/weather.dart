import 'package:json_annotation/json_annotation.dart';

part 'weather.g.dart';

@JsonSerializable(createToJson: false)
class Weather {
  final List<WeatherData> weather;
  @JsonKey(fromJson: _dateTimeFromSecondsSinceEpoch)
  final DateTime dt;
  final double temp;
  final int pressure;
  final int humidity;
  @JsonKey(name: 'wind_speed')
  final double windSpeed;

  Weather({
    required this.weather,
    required this.dt,
    required this.temp,
    required this.pressure,
    required this.humidity,
    required this.windSpeed,
  });

  factory Weather.fromJson(Map<String, dynamic> json) =>
      _$WeatherFromJson(json);

  static DateTime _dateTimeFromSecondsSinceEpoch(int seconds) {
    return DateTime.fromMillisecondsSinceEpoch(seconds * 1000, isUtc: true);
  }
}

@JsonSerializable(createToJson: false)
class WeatherData {
  final int id;
  final String main;
  final String description;
  final String icon;

  WeatherData({
    required this.id,
    required this.main,
    required this.description,
    required this.icon,
  });

  factory WeatherData.fromJson(Map<String, dynamic> json) =>
      _$WeatherDataFromJson(json);
}
