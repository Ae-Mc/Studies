import 'package:flutter/foundation.dart';
import 'package:weather_app/data/apis/apis.dart';
import 'package:weather_app/data/models/onecall_result.dart';
import 'package:weather_app/data/storage/storage.dart';

class WeatherProvider extends ChangeNotifier {
  late OneCallResult currentWeather;

  Future<void> initialize() async {
    await updateWeatherData();
  }

  Future<OneCallResult> updateWeatherData() async {
    return currentWeather = await APIs.openWeatherApi.weather(
      Storage.settings.activePlace.latitude,
      Storage.settings.activePlace.longitude,
    );
  }
}
