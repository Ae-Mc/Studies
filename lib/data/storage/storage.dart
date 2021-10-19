import 'dart:convert';

import 'package:shared_preferences/shared_preferences.dart';
import 'package:weather_app/data/models/place.dart';
import 'package:weather_app/data/models/settings.dart';

class Storage {
  static late SharedPreferences _sharedPreferences;
  static List<Place> favorites = [];
  static Settings settings = Settings();

  static Future<void> initialize() async {
    _sharedPreferences = await SharedPreferences.getInstance();
    favorites = (_sharedPreferences.getStringList('favorites') ?? [])
        .map((e) => Place.fromJson(jsonDecode(e)))
        .toList();
    final settingsString = _sharedPreferences.getString('settings');
    if (settingsString != null) {
      settings = Settings.fromJson(jsonDecode(settingsString));
    }
  }

  static Future<void> addFavorite(Place place) async {
    favorites.add(place);
    await saveFavorites();
  }

  static Future<void> removeFavorite(Place place) async {
    favorites.removeWhere((element) => element == place);
    await saveFavorites();
  }

  static Future<void> saveFavorites() async {
    await _sharedPreferences.setStringList(
      'favorites',
      favorites.map((e) => jsonEncode(e)).toList(),
    );
  }

  static Future<void> saveSettings() async {
    await _sharedPreferences.setString('settings', jsonEncode(settings));
  }

  Storage._();
}
