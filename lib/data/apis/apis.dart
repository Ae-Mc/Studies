import 'package:dio/dio.dart';
import 'package:weather_app/data/apis/geonames.dart';
import 'package:weather_app/data/apis/openweather.dart';

class APIs {
  static late GeoNamesApi geoNamesApi = GeoNamesApi(Dio(BaseOptions()));
  static late OpenWeatherApi openWeatherApi =
      OpenWeatherApi(Dio(BaseOptions()));

  APIs._();
}
