# Changelog of score2dx-gui

* 1.1.0 [2021-10-21] (score2dx-2.0.0):
    * Update to IIDX 29 CastHour.
    * Update for score2dx-2.0.0 change.
    * Improve **Graph**'s timeline (x-axis) display format from `yyyy` to `yyyy-MM`.
        * Rework timeline begin and end, now between filter version begin and 1 year of latest version begin, instead of hardcoded number.
    * Add **Timeline begin** filter to filter from specific version, up to IIDX 17 SIRIUS and default to IIDX 23 copula.
    * Rewrite README and add zh-TW version.

* 1.0.0 [2021-10-08] (score2dx-1.0.0):
    * Initial commit with basic functionalities.
        * UI for score2dx functions.
            * **Load directory** to load CSV files in a directory.
            * **Download from IST** to scrap data from [IST](https://score.iidx.app/) website.
        * **Graph** for historic score curve.
