# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- AttributeRange now accepts empty spaces (#21)
- Allows to zoom in/out with the alphanumeric keyboard (#28)

### Changed
- The `CellularAutomata1D` model plugin has been updated to implement the 256 elementary cellular automaton rules

### Fixed
- Fixes #27 - Experiment Designer: vertical scrollbar is hiding the buttons and fields
- Fixes MSVC2013 compilation

## [0.2.1] - 2018-10-23
### Added
- Improved the source code documentation.
- Adds the `Check for updates` button to the menu bar.

### Changed
- EvoplexCore is now licensed under the Apache 2.0 License terms.
- Update the `nowak92` plugin which is now called `prisonersDilemma`.

## [0.2.0] - 2018-09-04
### Added
- Adds support to edges attributes
- Increases unit-tests coverage of core components
- Implements the Bernoulli distribution in the PRG class
- Implements `AttributeRange::next()` and `AttributeRange::prev()`
- Plugins can now be reloaded
- Main GUI: Adds a status bar and a console to show the error messages
- Plugins page: Displays plugins location and meta data (#3)
- Projects page: Adds button to maximize graph widgets
- Projects page: Adds button to pause all experiments
- Experiments table: Allows hiding columns
- Graph widgets: Allows scaling the nodes and edges
- Graph widgets: Allows setting the edge with
- Graph widgets: Allows exporting nodes to a csv file
- Graph widgets: Allows exporting graph as image (svg, png, jpg and jpeg)
- Graph widgets: Allows zooming in/out with the mouse wheel
- Graph widgets: right click to toggle the attribute's value
- Graph widgets: Select node and hit `space` to toggle the attribute's value
- Graph widgets: `ctrl +` to zoom in, `ctrl -` to zoom out and `ctrl 0` to reset zoom
- Graph widgets: Allows moving graph with the arrow keys
- Graph widgets: Inspector has now a button to allow centralizing the selection
- Graph widgets: Inspector has now a refresh button
- Graph widgets: Allows selecting a node by id
- New model plugin: Population growth `populationGrowth`
- New model plugin: Conway's Game of Life cellular automaton `gameOfLife`
- New model plugin: Elementary cellular automaton rules `cellularAutomata1D`
- New graph plugin: Path graph (linear graph) (#1)
- New graph plugin: Cycle graph (#2)
- New graph plugin: Star graph
- New graph plugin: Zero edges - generates graphs without edges

### Changed
- Node's coordinates are now a float
- Graph widgets now use an OpenGL backend
- Graph widgets: Inspector now displays the neighbours' ids
- Improved look and feel of graph widgets
- Improved GraphView performance by avoiding caching invisible elements
- Attributes' inspector of the graph widgets now shows a proper field for each type of attribute
- Plugins page: minor GUI improvements
- Renamed the 'customGraph' plugin to 'edgesFromCSV'
- Appended the word `output` to all output attributes
- 'edgesFromCSV' plugin now supports edge attributes
- 'squareGuid' plugin now supports edge attributes

### Fixed
- Fixed bug when pausing an experiment with queued trials
- Close the ExperimentDesigner if there is no project opened
- `bool` attribute should act as a range and not as a single value
- Attrs generator should accept literal boolean, i.e., 'true' and 'false'
- Node::randNeighbour crashes when the neighbourhood is empty

### Removed
- PRG: replaced all `rand()` functions by `uniform()` and `bernoulli()`

## [0.1.0] - 2018-08-06
### Added
- Fixes #8 - Allows deleting an experiment
- Value class now accepts utf8 string
- Model plugins can now add and remove nodes and edges during the simulation
- Model plugins: added beforeLoop() and afterLoop() methods
- Plugins can now be versioned
- Increases unit-tests coverage of core components
- Adds new attribute range: "non-empty-string"
- ExpDesigner is now able to recognize "filepath" and "dirpath" attributes

### Changed
- Improves plugins initialization
- Improves and unify plugins interfaces
- Improves threads management
- Improves TableWidget performance

### Fixed
- Fixes packaging of all OSs
- Fixes logger on Windows
- Fixes #6 - Don't overwrite projects by default
- Fixes #7 - Checks for running experiments before closing a project
- Fixes #9 - Removes question mark from titlebar on Windows

## [0.1.0-alpha] - 2018-06-21
### Added
- First public release
