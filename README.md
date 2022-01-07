<p align="center">
  <a href="/doc/README_zh-TW.md">繁體中文 (Taiwanese Mandarin)</a>
</p>

# IIDX ScoreViewer

- ScoreViewer is a tool to visualize score data of player from Konami Beatmania IIDX game's CSV files (or third party score site).

- This GUI project utilize [score2dx](https://github.com/blazar0112/score2dx) library.

## Screenshot

![screenshot_1.5.0_graph](./doc/image/ScoreViewer-1.5.0_graph.png "ScoreViewer 1.5.0 Graph")
![screenshot_1.5.0_statistics](./doc/image/ScoreViewer-1.5.0_statistics.png "ScoreViewer 1.5.0 Statistics Table & ChartList")
![screenshot_1.5.0_activity](./doc/image/ScoreViewer-1.5.0_activity.png "ScoreViewer 1.5.0 Activity")

## Requirement

- Windows 10.

## How to use

- ScoreViewer provide 3 methods to load data:
    - Load from Konami CSV files.
    - Download from [IIDX ME (ME)](https://iidx.me/).
    - Download from [IIDX Score Table (IST)](https://score.iidx.app/).
- Service comparison:
    - ScoreViewer with CSV files:
        - :chart_with_upwards_trend: Loading is very fast.
        - :chart_with_upwards_trend: Backup data by yourself.
        - :chart_with_upwards_trend: Multiple CSV can capture score progress inside a version.
        - :chart_with_downwards_trend: :heavy_dollar_sign::heavy_dollar_sign: Requires e-amusement premium course subscription.
        - :chart_with_downwards_trend: Desktop application only. Not available as using website when playing.
    - Use ME directly or ScoreViewer with Download from ME:
        - :chart_with_upwards_trend: Can load previous existing data up to tricoro.
        - :chart_with_upwards_trend: Backup data using third party service.
        - :chart_with_upwards_trend: :heavy_dollar_sign: To update data can just use basic course subscription. (But slowly.)
        - :chart_with_upwards_trend: :heavy_dollar_sign::heavy_dollar_sign: Also support sync data to ME using CSV if have premium course subscription.
        - :chart_with_upwards_trend: Provide API for ScoreViewer to download user data in about 6 minutes.
        - :chart_with_downwards_trend: Not all score data detail is downloadable.
        - :chart_with_downwards_trend: Only keep final data of each previous versions.
        - :chart_with_downwards_trend: Risk for third party site to end service.
    - Use IST directly or ScoreViewer with Download from IST:
        - :chart_with_upwards_trend: Can load previous existing data (up to tricoro?).
            - Note: IST previously have sync IIDX ME function.
        - :chart_with_upwards_trend: Backup data using third party service.
        - :chart_with_upwards_trend: :heavy_dollar_sign: To update data can just use basic course subscription. (But slowly.)
        - :chart_with_upwards_trend: :heavy_dollar_sign::heavy_dollar_sign: Also support sync data to IST using CSV if have premium course subscription.
        - :chart_with_downwards_trend: Not all score data detail is downloadable.
        - :chart_with_downwards_trend: Only keep final data of each previous versions.
        - :chart_with_downwards_trend: Risk for third party site to end service.
        - :chart_with_downwards_trend: No API, ScoreViewer using scrapper to download IST data is extremely slow.
- How to load data from CSV files:
    - [IIDX 29 CSV download link](https://p.eagate.573.jp/game/2dx/29/djdata/score_download.html).
    - Save CSV files in a directory.
        - Directory must be named as IIDX ID.
            - For example: `5483-7391`.
        - Default filename is like `5483-7391_dp_score.csv`.
            - Recommend to rename and add date after `_score`.
            - Example: `5483-7391_dp_score_2020-11-21.csv`.
    - Click **Load Directory** and select above directory.
        - **Graph** shows your score curve chart.
        - Check [GUI Manual](#GUI-Manual) for detail.
    - ScoreViewer release provides example data in directory `Example/5483-7391` to try load directory.
    - **Load Directory** can also load exported data from third party score site (ME/IST).
- How to download data from [IIDX ME (ME)](https://iidx.me/):
    - Input your ME user name in right input field of **Enter User** and press `Enter`.
    - If ME have this user will display in **ME User** and corresponding **IIDX ID**.
    - Click **Download from ME**, which will be enabled when ME's **IIDX ID** is not empty.
    - Wait about 6~7 minutes (see progress bar).
    - Download data is exported to `ME/<IIDX_ID>` directory, and will load automatically after downloaded.
    - Can later just use **Load Directory** to load that directory.
- How to download data from [IIDX Score Table (IST)](https://score.iidx.app/):
    - Check [download from IST requirements](#Download-from-IST-requirements) before running script.
    - Input your IIDX ID in right side of **Add Player**, and press `Enter` to add to **IIDX ID** list.
    - Input versions and styles of your data, comma-separated.
    - Default **Versions** is `28, 29`, you can modify versions showed in your IST user page.
        - [Example IST User Page](https://score.iidx.app/users/5483-7391), in this case modify to `26, 27, 28`.
    - Default **Styles** is `SP, DP`, you can modify if you only want one style's data.
    - Click **Download from IST** to download current selected IIDX ID data from IST.
        - It runs in background.
        - Button is disabled during download.
        - Recommend to use **Run in PowerShell** mode, which will pop a PowerShell window executing `ist_scraper.exe`, shows progress and close after finish so you can know download is completed.
        - Takes about 1 minute to start the background download executable.
        - Take about 5 minutes per version-style to download data (intended delay to not be recognized as attacking IST).
        - Faster if you have fewer data.
        - For example: using setting, `27, 28` and `SP, DP`, takes about 20 minutes to download.
        - Download data is exported to `IST/<IIDX_ID>` directory, and will load automatically after downloaded.
        - Can later just use **Load Directory** to load that directory.
- Data can be combined from all sources:
    - For example:
        - Have synced IIDX ME to IST during 24.
        - Used IST javascript during 26, 27, 28 before.
        - Use ScoreViewer to download from IST with **Versions** = `24, 26, 27, 28`.
        - Use ScoreViewer to load 29 CSV.
        - **Graph** should display 24 to 29 curve.

## Important Notice

- Be polite to third party website (ME/IST):
    - You should only download from ME/IST once in a while for your data and only load export data afterwards if you can use CSV approach.
    - Do not use this to scrap ME/IST frequently and/or massive scrap other's data.

## Download from IST requirements

- Using Python script packed as `ist_scraper.exe` to drive Chrome to scrap IST website data and assumed following requirements:
    - Chrome browser installed in `C:\Program Files`.
    - Chrome browser version same as chromedriver.
        - `ist_scraper.exe` will check version for you, you can see if version mismatch in PowerShell mode.
    - Current release bundle a v96 `chromedriver.exe`.
        - Download chromedriver from [official download](https://chromedriver.chromium.org/downloads) and to match your Chrome version if needed, since Chrome may update or you did not update your Chome.
            - Replace bundled `chromedriver.exe` with downloaded one in same directory of `ScoreViewer.exe`.

## GUI Manual

- Purple UI section bar with triangle `▼`: click section bar to collapse below section.
- Green background ComboBox: click and select items in dropdown menu.
    - **IIDX ID**
    - **Play Style**
    - **Difficulty**
    - **Timeline begin**
- **IIDX ID** list:
    - Current loaded players, identified by IIDX ID.
    - IIDX ID format: `5483-7391`.
    - Selected IIDX ID is current player, used in **Download from IST** and **Graph**.
    - **Add Player** can manually add IIDX ID to list.
        - GUI allows you to input without dash `-`.
        - Press `Enter` to input, if format is correct the input field is cleared.
    - After **Load Directory**, automatically add loaded directory to list, because the directory must be named as IIDX ID.
        - So you do not need to manually **Add Player** while load with existing data.
- **Load Directory**:
    - Load score data from directory named as IIDX ID.
    - Score data is parsed from files inside directory:
        - CSV files, e.g. `5483-7391_dp_score_2020-11-21.csv`.
            - Any character between `_score` and `.csv` is ignored, so you can use this to annotate CSV.
        - Export files, e.g. `score2dx_export_SP_2021-09-14_IST_28.json`.
    - Not support unicode filename currently, filename must consist of alphanumeric and system allowed ASCII symbols.
- **View Setting**:
    - Setting that affect multiple area:
        - **ActiveVersion**: affect Statistics, selecting active version will have statistics calculated as time back at end date of that version.
        - **Play Style**: affect Graph and Statistics.
            - Can press `Key S` or `Key D` to select SP/DP using keyboard.
        - **Difficulty**: only affect Graph.
- **Download from IST**:
    - Already covered in [How to use](#How-to-use).

- **Graph**:
![screenshot_1.5.0_graph](./doc/image/ScoreViewer-1.5.0_graph.png "ScoreViewer 1.5.0 Graph")
    - Use left side **Play Style** and **Difficulty** to select style-difficulty data.
    - Selecting music:
        - Click **Version Folder** (cyan color button) to expand and collapse musics inside that version.
        - `Wheel up` and `Wheel down` to scroll version folders.
        - Above version folders, right top two buttons:
            - **Collapse All Versions**, left one.
            - **Expand All Versions**, right one.
                - Expand all also centers current selected music.
        - Current selected music: pinkish background.
        - Click unselected music (gray background) to select music.
            - Can also use key to navigate:
                - `Arrow Up` to jump one music upwards.
                - `Arrow Down` to jump one music downwards.
                - `Page Up` to jump 5 music upwards.
                - `Page Down` to jump 5 music downwards.
                - It's circular from top-most to down-most.
                    - But current GUI implementation may have bug sometimes when jump from top-most to down-most, musics will go out of display range.
                    - Click **Expand All Versions** to workaround in this case, should center correctly.
    - Score chartview:
        - X axis: timeline of score.
            - Top separate axis by each version.
            - Bottom denotes year-month of 1/10 axis.
        - Y axis: EX score value.
            - Right side denotes score value for each DJ level.
        - Red point and curve line:
            - X: Date time.
            - Y: EX score value with white label.
        - If score is updated or is latest, add indicator above score point:
            - Update means better than previous data in any of `clear, EX score, DJ level`.
            - Clear: yellow dot above with clear lamp and clear type text.
            - Score: green dot above with DJ level text and difference from closest DJ level.
                - For example: `AA(AA+2)`.
        - **Timeline begin**: select chartview begin version, so timeline matches your data properly.
            - Default version: `IIDX 23 copula`, earliest of CSV service.
            - Selectable range: `IIDX 17 SIRIUS` to `IIDX 28 BISTOVER`.

- **Statistics**:
![screenshot_1.5.0_statistics](./doc/image/ScoreViewer-1.5.0_statistics.png "ScoreViewer 1.5.0 Statistics Table & ChartList")
    - Show statistics table, which affected by
        - ActiveVersion
        - Table
        - Column
        - Value
    - Table also sum up each row and column in addition.
    - Total: count all available charts for each row.
    - Active version: calculate statistics from data during that version.
        - Not only lookup from player data, but also consider music/chart availability.
    - Table: select row type and data source
        - Level:
            - Row is level 1 to level 12.
        - All difficulty:
            - Row is difficulty (excludes Beginner).
        - Difficulty by Version:
            - Row is difficulty (excludes Beginner).
            - Table is filtered to only include music of selected version.
    - Column: select column type
        - By Clear:
            - NO PLAY
            - FAILED
            - ASSIT
            - EASY
            - CLEAR
            - HARD
            - EX HARD
            - FC
        - By DJ Level:
            - F
            - E
            - D
            - C
            - B
            - A
            - AA
            - AAA
        - By Score Level Category:
            - A-
            - A+
            - AA-
            - AA+
            - AAA-
            - AAA+
            - MAX-
            - MAX
    - Value: table cell display type
        - Count
        - Percentage (divided by row total chart count)
- **Statistics Table**
    - Can click heading section `Statistics Table` bar to expand/collapse table (to view more chart list rows).
    - Click each non-zero cell (white background) will display charts from that cell in chart list.
- **Statistics Chart List**
    - Display score information of filtered charts.
    - Show filters in heading section `Chart List` by green tags.
    - Columns of chart list:

        | Column | Explanation | Note |
        | - | - | - |
        | Ver | Version of chart's music. | |
        | C | ClearType displayed as clear lamp. | |
        | Lv | Level of chart. | |
        | Title | Title of chart's music. | |
        | DJ Level | DJ level of chart's score. | `F` if `NO PLAY`. |
        | Score | EX Score of chart. | `0` if `NO PLAY`. |
        | SL Diff | Difference from Score Level of chart's score.<br>e.g. `MAX-20` | `NP` if `NO PLAY`. |
        | Miss | Miss count of chart's score. | `N/A` if `NO PLAY`. |
        | PDBS Diff | Difference of Score from `Personal Diffable Best Score`, which is best or second-best score (if current is Personal Best). | `NP` if `NO PLAY`, `PB` if no any PDBS record available. <br>Colored green if better (`+`, increased), red if worser (`-`, decreased). |
        | PDBS Ver | Version of `Personal Diffable Best Score` record datetime (not music's). | `N/A` if no any PDBS record available. |
        | PDB Score | Value of `Personal Diffable Best Score`. | `N/A` if no any PDBS record available. |
        | PDBM Diff | Difference of Miss Count from `Personal Diffable Best Miss`, which is best or second-best miss (if current is Personal Best). | `NP` if `NO PLAY`, `PB` if no any PDBM record available. <br>Colored green if better (`-`, decreased), red if worser (`+`, increased). |
        | PDBM Ver | Version of `Personal Diffable Best Miss` record datetime (not music's). | `N/A` if no any PDBM record available. |
        | PDB Miss | Value of `Personal Diffable Best Miss`. | `N/A` if no any PDBM record available. |

        - Note: `Diffable` means must have difference in value, so same value records are ignored, therefore Diffable record may not be available.

- **Activity**:
![screenshot_1.5.0_activity](./doc/image/ScoreViewer-1.5.0_activity.png "ScoreViewer 1.5.0 Activity")
- **Calendar**
    - Click date to view activity.
    - Date with activity display in yellow text.
    - Switch active version jump to version begin date.
    - **Button <**: jump to previous month.
    - **Button >**: jump to next month.
    - **Button T**: jump to today.
    - Date not in version but with activity can still click and show activity list.
        - But it may not compare to correct Personal Best data.
        - Music/chart may be deleted/revived/added so not include in list correctly.
        - Please use active version to show activity in that version and click yellow dates.
- **Activity List**
    - Show each music activity in selected date.
    - Activity: music with update time in selected date.
        - Music may have different chart scores updating, but sum up in single music activity.
    - Activity time is last update time in CSV/imported data, please note that it does not reflect actual play history.
        - e.g.
            - Play one music 6 times, CSV only record last update time.
            - Download CSV two different time in a day, music can appear at list with two time point from each CSV.
    - Play Count: music play count, **not** player play count (credit count).
        - CSV only records each music play count.
        - Can divide by 4 to estimate player play count.
    - If data is downloaded from IST: datetime is script running time, and play count is zero.
    - New Record: only shows column with updated value.
    - PDBS/PDBM diff: same as in statistics, see explanation in **Statistics Chart List**.
