import 'package:dio/dio.dart';
import 'package:weather_app/data/apis/geonames.dart';

class APIs {
  static late GeoNamesApi geoNamesApi = GeoNamesApi(Dio(BaseOptions()));

  APIs._();
}
