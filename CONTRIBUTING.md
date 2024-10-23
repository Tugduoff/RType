# CONTRIBUTING
Here you can find informations on how you could contribute to the project

## Compile the project
To compile the project, you must have CMake installed on your machine and then launch the build script corresponding to your operating system :

#### Windows
```powershell
.\windows_build_rtype.bat
```

#### Linux
```bash
./build_rtype.sh
```

You will then have either a `r-type_server` and a `r-type_client` for linux, or a `r-type_server.exe` and a `r-type_client.exe` for windows.

## Pull requests
If you want to contribute to the project, you should do so with a pull request.

If you want your pull request to be validated, it should respect the following conditions :
- The description of the pull request **must** be self-sufficient and **must** explain what is implemented in it. You should also mention issues related to the pull request (if any) in the description (e.g. `close #23`).
- The code **must** be documented with Doxygen-like comment. All classes, methods and functions must have their documentation to explain what they does.
- The commits **must** respect the [commit norm](#commit-norm) mentionned below.

If you respected all the conditions listed, one of the current maintainers of the project will read your pull request and either approve or request changes.

## Commit norm
### Repository changes
- 🔨 `:hammer:` : CMake-related
- 🔝 `:top:` : Header files
- 📚 `:books:` : Lib-related changes

- ✅ `:white_check_mark:` : Add, update or pass tests.
- 🧪 `:test_tube:` : Add a failing test

- 📝 `:memo:` : Documentation
- 🙈 `:see_no_evil:` : Gitignore
- 📦 `:package:` : Package.json
- 🚚 `:truck:` : Move or rename resources
- 🔥 `:fire:` : Remove files
- 🔀 `:twisted_rightwards_arrows:` : Merge branch

- 👷 `:construction_worker:` : Add or update automation tools
- 💚 `:green_heart:` : Fix CI build

### Code changes
- ✨ `:sparkles:` : New feature of the project
- ✨✨ `:sparkles::sparkles:` : Major New features

- 🐛 `:bug:` : Bug fixes
- 🚑 `:ambulance:` : Critical hotfixes
- 🔧 `:wrench:` : General fix for something that wasn't working as expected
- 🚨 `:rotating_light:` Fix compiler warnings

- ➕ `:heavy_plus_sign:` : New file, new function
- ➖ `:heavy_minus_sign:` : Deletion of some code
- ♻️ `:recycle:` : Refactor code

### Extra
- 🚀 `:rocket:` : Final commit

## Releases

Here we will explain how the files located in the [release page](https://github.com/Tugduoff/RType/releases) of Github are produced

#### Windows
On windows we simply launch the build script (`windows_build_rtype.bat`), and then zip the produced bin folder.

#### Linux
On Linux you have to launch the build script named `build_and_fetch_binaries.sh` which will build the project inside a Debian and Fedora docker container and which will the get the .tar archive produced.
