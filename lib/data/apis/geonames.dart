import 'package:dio/dio.dart';
import 'package:retrofit/retrofit.dart';
import 'package:weather_app/data/models/search_result.dart';

part 'geonames.g.dart';

@RestApi(baseUrl: 'http://api.geonames.org/')
abstract class GeoNamesApi {
  factory GeoNamesApi(Dio dio, {String? baseUrl}) = _GeoNamesApi;

  @GET('searchJSON')
  Future<SearchResult> search(
    @Query('name', encoded: true) String name, {
    @Query('lang') String language = 'ru',
    @Query('username') String username = 'ae_mc',
    @Query('orderby') String orderBy = 'population',
    @Query('fuzzy') double fuzzy = 1,
    @Query('maxRows') int maxRows = 10,
  });
}
