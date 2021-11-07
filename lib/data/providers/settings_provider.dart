import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:weather_app/data/models/place.dart';
import 'package:weather_app/data/models/settings.dart';
import 'package:weather_app/data/storage/storage.dart';

extension ParseTempToString on TemperatureUnits {
  String get inString => ['˚c', '˚F'][index];
}

extension ParseSpeedToString on SpeedUnits {
  String get inString => ['м/с', 'км/ч'][index];
}

extension ParsePressureToString on PressureUnits {
  String get inString => ['мм.рт.ст', 'гПа'][index];
}

class SettingsProvider extends ChangeNotifier implements ReassembleHandler {
  TemperatureUnits get temperatureUnits => Storage.settings.temperatureUnits;
  SpeedUnits get speedUnits => Storage.settings.speedUnits;
  PressureUnits get pressureUnits => Storage.settings.pressureUnits;
  ThemeMode get themeMode => Storage.settings.themeMode;
  Place get activePlace => Storage.settings.activePlace;

  set temperatureUnits(TemperatureUnits newValue) => notify(
        () {
          Storage.settings.temperatureUnits = newValue;
          Storage.saveSettings();
        },
      );
  set speedUnits(SpeedUnits newValue) => notify(
        () {
          Storage.settings.speedUnits = newValue;
          Storage.saveSettings();
        },
      );
  set pressureUnits(PressureUnits newValue) => notify(
        () {
          Storage.settings.pressureUnits = newValue;
          Storage.saveSettings();
        },
      );
  set themeMode(ThemeMode newValue) => notify(
        () {
          Storage.settings.themeMode = newValue;
          Storage.saveSettings();
        },
      );
  set activePlace(Place newValue) => notify(
        () {
          Storage.settings.activePlace = newValue;
          Storage.saveSettings();
        },
      );

  void switchTemperatureUnits() {
    temperatureUnits = temperatureUnits == TemperatureUnits.celcius
        ? TemperatureUnits.farenheit
        : TemperatureUnits.celcius;
  }

  void switchSpeedUnits() {
    speedUnits = speedUnits == SpeedUnits.metersPerSecond
        ? SpeedUnits.kilometersPerHour
        : SpeedUnits.metersPerSecond;
  }

  void switchPressureUnits() {
    pressureUnits = pressureUnits == PressureUnits.hectopascal
        ? PressureUnits.mmOfMercury
        : PressureUnits.hectopascal;
  }

  void switchTheme() {
    themeMode = themeMode == ThemeMode.dark ? ThemeMode.light : ThemeMode.dark;
  }

  void notify(Function f) {
    f();
    notifyListeners();
  }

  @override
  void reassemble() {}
}
