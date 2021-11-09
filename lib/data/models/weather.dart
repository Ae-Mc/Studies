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
  final WeatherIcon icon;

  WeatherData({
    required this.id,
    required this.main,
    required this.description,
    required this.icon,
  });

  factory WeatherData.fromJson(Map<String, dynamic> json) =>
      _$WeatherDataFromJson(json);
}

enum WeatherIcon {
  @JsonValue('01d')
  d01,
  @JsonValue('01n')
  n01,
  @JsonValue('02d')
  d02,
  @JsonValue('02n')
  n02,
  @JsonValue('03d')
  d03,
  @JsonValue('03n')
  n03,
  @JsonValue('04d')
  d04,
  @JsonValue('04n')
  n04,
  @JsonValue('09d')
  d09,
  @JsonValue('09n')
  n09,
  @JsonValue('10d')
  d10,
  @JsonValue('10n')
  n10,
  @JsonValue('11d')
  d11,
  @JsonValue('11n')
  n11,
  @JsonValue('13d')
  d13,
  @JsonValue('13n')
  n13,
  @JsonValue('50d')
  d50,
  @JsonValue('50n')
  n50,
}
