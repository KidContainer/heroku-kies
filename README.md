# kies-heroku-fullstack

![CMake](https://github.com/KieQ/kies-heroku-fullstack/workflows/CMake/badge.svg)

**This repository has been splited into two repositories and no longer maintained.**

**For detail, please check [KieQ/kies-code-backend](https://github.com/KieQ/kies-code-backend) and [KieQ/kie-code-frontend](https://github.com/KieQ/kies-code-frontend)**

This is the source code for kies on heroku. It's a HTTP server which provide some calculation task for `kies`.

|Folder Name|Explanation|
|----|----|
|db| The db operation source code|
|dto| The type to represent a kind of json structure|
handler| The bussiness logistic source code|
|lib| The third party libraries which are not provided by conan, only for C++ part|
|web| The source code of HTML, CSS, JavaScript and other resources used by front end part|
|utils| The utility source code of C++ part|
|www| The static file folder for the whole application|
