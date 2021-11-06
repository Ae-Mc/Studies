import 'package:json_annotation/json_annotation.dart';
import 'package:weather_app/data/models/day_weather.dart';
import 'package:weather_app/data/models/weather.dart';

part 'onecall_result.g.dart';

@JsonSerializable(createToJson: false)
class OneCallResult {
  final Weather current;
  final List<Weather> hourly;
  final List<DayWeather> daily;

  OneCallResult({
    required this.current,
    required this.hourly,
    required this.daily,
  });

  factory OneCallResult.fromJson(Map<String, dynamic> json) =>
      _$OneCallResultFromJson(json);
}
