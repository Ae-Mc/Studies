import 'package:flutter/foundation.dart';
import 'package:weather_app/data/apis/apis.dart';
import 'package:weather_app/data/models/onecall_result.dart';
import 'package:weather_app/data/models/settings.dart';
import 'package:weather_app/data/models/weather.dart';
import 'package:weather_app/data/providers/settings_provider.dart';
import 'package:weather_app/gen/assets.gen.dart';

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

  static AssetGenImage getWeatherIconAssetFromWeatherDataIcon(
      WeatherIcon icon) {
    switch (icon) {
      case WeatherIcon.d01:
      case WeatherIcon.n01:
        return Assets.icons.universal.sun;
      case WeatherIcon.d02:
      case WeatherIcon.n02:
        return Assets.icons.universal.partlyCloudy;
      case WeatherIcon.d03:
      case WeatherIcon.n03:
      case WeatherIcon.d04:
      case WeatherIcon.n04:
        return Assets.icons.universal.cloudy;
      case WeatherIcon.d09:
      case WeatherIcon.n09:
        return Assets.icons.universal.rain;
      case WeatherIcon.d10:
      case WeatherIcon.n10:
        return Assets.icons.universal.rain3Drops;
      case WeatherIcon.d11:
        return Assets.icons.universal.thunderstorm;
      case WeatherIcon.n11:
        return Assets.icons.universal.thunderstorm;
      case WeatherIcon.d13:
      case WeatherIcon.n13:
        return Assets.icons.universal.snowy;
      case WeatherIcon.d50:
      case WeatherIcon.n50:
        return Assets.icons.universal.mist;
    }
  }
}
