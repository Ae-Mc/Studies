import 'package:flutter/foundation.dart';
import 'package:weather_app/data/apis/apis.dart';
import 'package:weather_app/data/models/onecall_result.dart';
import 'package:weather_app/data/models/settings.dart';
import 'package:weather_app/data/providers/settings_provider.dart';

class WeatherProvider extends ChangeNotifier {
  late OneCallResult currentWeather;
  late SettingsProvider settings;

  Future<void> initialize() async {
    await updateWeatherData();
  }

  Future<OneCallResult> updateWeatherData() async {
    currentWeather = await APIs.openWeatherApi.weather(
      settings.activePlace.latitude,
      settings.activePlace.longitude,
    );
    notifyListeners();
    return currentWeather;
  }

  double getTempInCurrentUnits(double celciusTemp) {
    if (settings.temperatureUnits == TemperatureUnits.farenheit) {
      return celciusTemp * 1.8 + 32;
    }
    return celciusTemp;
  }

  double getSpeedInCurrentUnits(double metricSpeed) {
    if (settings.speedUnits == SpeedUnits.kilometersPerHour) {
      return metricSpeed * 3.6;
    }
    return metricSpeed;
  }

  double getPressureInCurrentUnits(int pressureInHectopascal) {
    if (settings.pressureUnits == PressureUnits.mmOfMercury) {
      return pressureInHectopascal * 0.7500638;
    }
    return pressureInHectopascal.toDouble();
  }

  String getPressure(int pressureInHectopascal, [int fractionDigits = 1]) {
    return getPressureInCurrentUnits(pressureInHectopascal)
            .toStringAsFixed(fractionDigits) +
        settings.pressureUnits.inString;
  }
}
