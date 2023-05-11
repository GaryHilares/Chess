<h1 align="center">Chess</h1>

<p align="center">
  <img alt="badge-lastcommit" src="https://img.shields.io/github/last-commit/GaryHilares/Chess?style=for-the-badge">
  <img alt="badge-openissues" src="https://img.shields.io/github/issues-raw/GaryHilares/Chess?style=for-the-badge">
  <img alt="badge-license" src="https://img.shields.io/github/license/GaryHilares/Chess?style=for-the-badge">
  <img alt="badge-contributors" src="https://img.shields.io/github/contributors/GaryHilares/Chess?style=for-the-badge">
  <img alt="badge-codesize" src="https://img.shields.io/github/languages/code-size/GaryHilares/Chess?style=for-the-badge">
</p>

<p align="center">
  <img alt="chess" src="https://user-images.githubusercontent.com/46727048/129751762-0249c064-0756-463d-af85-ac5d3f896380.png">
</p>

## Useful links
- [Project's Style Guide (WebKit)](https://webkit.org/code-style-guidelines/)

## What is Chess?
Chess is a virtual chess board that you can use to play this thought-provoking sport.

### Features
- **Detects illegal moves:** If you mistakenly play a piece to a square you're not allowed, the piece will automatically return to its place. Supports en-passant, promotion and castling, of course.
- **Free & open source:** You don't have to pay for anything. Really! Moreover, you have access to the code, so you can tweak anything you'd like.

### Platforms
- Cross-platform

### Dependencies
#### Development
- SFML

## Motivation
I like chess, so I made this game for fun and to practice my C++ skills.

## Installation and usage
You can build the project from source by following the next steps:
1. Prepare the external libraries:
  1. Get SFML 2.5.1.
    - You can look at how to get SFML [here](https://www.sfml-dev.org/tutorials/2.5/).
  2. Order the external libraries:
    1. Create a folder called `extlibs` in the root folder.
    2. Add the external libraries here. The minimal `extlibs` folder should look like this:
    ```
    | extlibs/
    |--- SFML-2.5.1/
    |------- bin/
    |------- include/
    |------- lib/
    ```
2. Build Chess:
  1. Fork the repository on GitHub.
  2. Go to your fork of the repository and copy the link to clone your repository.
  3. Go to Git on your local machine and use the command `git clone (your link)`.
  4. Set the variable CMAKE_BUILD_TYPE in `CMakefiles.txt` to "Release" or to "Debug" depending on what you want to build.
  5. Use CMake to generate makefiles from `CMakelists.txt`.
  6. Use the generated makefiles to build Chess.

After doing this, Chess should appear inside a folder in `build/`.

You can run the executable from the command line or by double clicking it. You're ready to play!

## Contributors
Thanks to these wonderful people for making Chess possible!

<p align="center"><a href="https://github.com/GaryHilares/Chess/graphs/contributors"><img src="https://contrib.rocks/image?repo=GaryHilares/Chess"></a></p>

## License
This work is licensed under a [Creative Commons Attribution 4.0 International License](https://github.com/GaryHilares/Chess/blob/main/LICENSE).

