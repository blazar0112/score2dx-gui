# Changelog of score2dx-gui

- Ongoing:
    - Add Activity tab.
    - score2dx fixes:
        - Non-FC Miss = 0 from import data should be recognized as failed without miss.
        - Revival music mistakenly inherit clear mark before. Should reset to NO PLAY after revival.

- 1.3.0 [2021-11-17] (score2dx-2.4.0):
    - Statistics table add collapsible heading and adjust resize behavior.
    - Add global hotkey to select SP/DP (Key S and Key D).
    - Can click statistics table cell now and show chart list of that cell.
    - Add Chart List UI.
    - Update README.

- 1.2.0 [2021-11-12] (score2dx-2.3.0):
    - Update DB to 29048 [2021-11-12].
    - Add GUI for score2dx added Statistics and ActiveVersion feature.
        - Use ActiveVersion to get ScoreAnalysis at that time.
        - **Statistics** tab offer multiple options to view ScoreAnalysis.
    - Redesign UI layout.
        - Rework side bar and can collopase ui now.
    - Add window icon.
    - Window title now shows IIDX ID.
    - Update README.

- 1.1.0 [2021-10-21] (score2dx-2.0.0):
    - Update to IIDX 29 CastHour.
    - Update for score2dx-2.0.0 change.
    - Improve **Graph**'s timeline (x-axis) display format from `yyyy` to `yyyy-MM`.
        - Rework timeline begin and end, now between filter version begin and 1 year of latest version begin, instead of hardcoded number.
    - Add **Timeline begin** filter to filter from specific version, up to IIDX 17 SIRIUS and default to IIDX 23 copula.
    - Rewrite README and add zh-TW version.

- 1.0.0 [2021-10-08] (score2dx-1.0.0):
    - Initial commit with basic functionalities.
        * UI for score2dx functions.
            - **Load directory** to load CSV files in a directory.
            - **Download from IST** to scrap data from [IST](https://score.iidx.app/) website.
        - **Graph** for historic score curve.
