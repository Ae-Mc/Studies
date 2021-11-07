import 'package:flutter/material.dart';
import 'package:json_annotation/json_annotation.dart';
import 'package:weather_app/data/models/place.dart';

part 'settings.g.dart';

@JsonSerializable()
class Settings {
  TemperatureUnits temperatureUnits;
  SpeedUnits speedUnits;
  PressureUnits pressureUnits;
  ThemeMode themeMode;
  Place activePlace;

  Settings({
    this.temperatureUnits = TemperatureUnits.celcius,
    this.speedUnits = SpeedUnits.metersPerSecond,
    this.pressureUnits = PressureUnits.mmOfMercury,
    this.themeMode = ThemeMode.system,
    this.activePlace = const Place(
      name: 'Санкт-Петербург',
      latitude: 59.93863,
      longitude: 30.31413,
      population: -1,
    ),
  });

  factory Settings.fromJson(Map<String, dynamic> json) =>
      _$SettingsFromJson(json);

  Map<String, dynamic> toJson() => _$SettingsToJson(this);
}

enum TemperatureUnits { celcius, farenheit }
enum SpeedUnits { metersPerSecond, kilometersPerHour }
enum PressureUnits { mmOfMercury, hectopascal }
