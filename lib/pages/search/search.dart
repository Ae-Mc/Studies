import 'package:assorted_layout_widgets/assorted_layout_widgets.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:weather_app/data/apis/apis.dart';
import 'package:weather_app/data/models/place.dart';
import 'package:weather_app/data/storage/storage.dart';

class SearchPage extends StatefulWidget {
  const SearchPage({Key? key}) : super(key: key);

  @override
  State<SearchPage> createState() => _SearchPageState();
}

class _SearchPageState extends State<SearchPage> {
  final _searchController = TextEditingController();
  var _clearButtonShowed = false;
  late List<Place> places;

  @override
  void initState() {
    places = Storage.favorites;
    _searchController.addListener(() {
      if (_searchController.text.isNotEmpty != _clearButtonShowed) {
        setState(() {
          _clearButtonShowed = _searchController.text.isNotEmpty;
        });
      }
      if (_searchController.text.isNotEmpty) {
        updateCities();
      }
    });
    super.initState();
  }

  Future<void> updateCities() async {
    var results =
        (await APIs.geoNamesApi.search(_searchController.text)).geonames;
    places = results.toSet().toList();
    setState(() {});
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: SafeArea(
        child: Padding(
          padding: const Pad(top: 32, horizontal: 20),
          child: Column(
            children: [
              Row(
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  IconButton(
                    onPressed: () => Navigator.of(context).pop(),
                    icon: Icon(
                      Icons.arrow_back_ios_rounded,
                      color: Theme.of(context).colorScheme.onBackground,
                    ),
                  ),
                  const SizedBox(width: 24),
                  Expanded(
                    child: TextField(
                      controller: _searchController,
                      style: Theme.of(context).textTheme.overline,
                      cursorColor: Theme.of(context).colorScheme.onBackground,
                      decoration: InputDecoration(
                        hintText: "Введите название города...",
                        suffixIcon: AnimatedSwitcher(
                          duration: const Duration(milliseconds: 300),
                          child: _clearButtonShowed
                              ? InkWell(
                                  customBorder: const StadiumBorder(),
                                  onTap: () => _searchController.clear(),
                                  child: Icon(
                                    CupertinoIcons.clear_circled_solid,
                                    color: Theme.of(context)
                                        .colorScheme
                                        .onBackground,
                                  ),
                                )
                              : const SizedBox(),
                        ),
                      ),
                    ),
                  )
                ],
              ),
              const SizedBox(height: 24),
              Expanded(
                child: ListView.separated(
                  itemCount: places.length,
                  itemBuilder: (context, index) =>
                      cityRow(context, places.elementAt(index)),
                  separatorBuilder: (_, __) => const Divider(
                    height: 1,
                  ),
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget cityRow(BuildContext context, Place place) {
    final isFavorite = Storage.favorites.contains(place);
    return ListTile(
      dense: true,
      onTap: () {
        (isFavorite
                ? Storage.removeFavorite(place)
                : Storage.addFavorite(place))
            .then((_) => setState(() {}));
      },
      contentPadding: Pad.zero,
      title: Text(
        place.country == null ? place.name : '${place.country}, ${place.name}',
        style: Theme.of(context).textTheme.overline,
      ),
      trailing: Icon(
        isFavorite ? Icons.star_rounded : Icons.star_border_rounded,
        color: Theme.of(context).colorScheme.onBackground,
      ),
    );
  }
}
