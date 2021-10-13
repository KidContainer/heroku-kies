# heroku-kies

![CMake](https://github.com/KieQ/heroku-kies/workflows/CMake/badge.svg)

This is the source code for kies on heroku. It's a HTTP server which provide some calculation task for `kies`.

|Folder Name|Explanation|
|----|----|
|db| The db operation source code|
|dto| The type to represent a kind of json structure|
handler| The bussiness logistic source code|
|lib| The third party libraries which are not provided by conan, only for C++ part|
|src| The source code of HTML, CSS, JavaScript and other resources used by front end part|
|utils| The utility source code of C++ part|
|www| The static file folder for the whole application|
