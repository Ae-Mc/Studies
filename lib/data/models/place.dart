import 'package:json_annotation/json_annotation.dart';

part 'place.g.dart';

@JsonSerializable()
class Place {
  @JsonKey(name: 'countryName')
  final String? country;
  @JsonKey(name: 'lat', fromJson: double.parse)
  final double latitude;
  @JsonKey(name: 'lng', fromJson: double.parse)
  final double longitude;
  final String name;
  final int population;

  factory Place.fromJson(Map<String, dynamic> json) => _$PlaceFromJson(json);
  Map<String, dynamic> toJson() => _$PlaceToJson(this);

  const Place({
    this.country,
    required this.latitude,
    required this.longitude,
    required this.name,
    required this.population,
  });
}
