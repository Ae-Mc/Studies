import 'package:json_annotation/json_annotation.dart';
import 'package:weather_app/data/models/place.dart';

part 'search_result.g.dart';

@JsonSerializable(createToJson: false)
class SearchResult {
  final int totalResultsCount;
  final List<Place> geonames;

  SearchResult(this.totalResultsCount, this.geonames);

  factory SearchResult.fromJson(Map<String, dynamic> json) =>
      _$SearchResultFromJson(json);
}
