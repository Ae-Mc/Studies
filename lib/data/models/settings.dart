import 'package:flutter/material.dart';
import 'package:json_annotation/json_annotation.dart';

part 'settings.g.dart';

@JsonSerializable()
class Settings {
  TemperatureUnits temperatureUnits;
  SpeedUnits speedUnits;
  PressureUnits pressureUnits;
  ThemeMode themeMode;

  Settings({
    this.temperatureUnits = TemperatureUnits.celcius,
    this.speedUnits = SpeedUnits.metersPerSecond,
    this.pressureUnits = PressureUnits.mmOfMercury,
    this.themeMode = ThemeMode.system,
  });

  factory Settings.fromJson(Map<String, dynamic> json) =>
      _$SettingsFromJson(json);

  Map<String, dynamic> toJson() => _$SettingsToJson(this);
}

enum TemperatureUnits { celcius, farenheit }
enum SpeedUnits { metersPerSecond, kilometersPerHour }
enum PressureUnits { mmOfMercury, gigaPascal }
