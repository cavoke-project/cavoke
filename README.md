<a href="https://github.com/cavoke-project/cavoke">
  <img align="left" height="80px" src="https://avatars.githubusercontent.com/u/52053547?s=200&v=4">
</a>
<h1 style="display: inline;">
  Cavoke
</h1>

<p>Платформа для создания и проведения сетевых пошаговых настольных игр</p>

[![CircleCI Build Status](https://img.shields.io/circleci/build/github/cavoke-project/cavoke?style=flat-square)](https://app.circleci.com/pipelines/github/cavoke-project/cavoke?filter=all)
[![GitHub top language](https://img.shields.io/github/languages/top/cavoke-project/cavoke?logo=github&style=flat-square)](https://github.com/cavoke-project/cavoke)
[![license](https://img.shields.io/github/license/cavoke-project/cavoke?style=flat-square)](./LICENSE)

## Что это
Cavoke – платформа для создания и проведения сетевых пошаговых настольных игр: позволяет разработчику не думать о создании комнат, подключении игроков, передаче данных от сервера к клиентам. Состоит из сервера и игрового клиента (отвечает за запуск интерфейсов игр). 

Разработчик игры регистрирует свой игровой продукт на нашей платформе, предоставляет компоненты с серверной и игровой логикой. Игрок, посредством приложения Cavoke клиента, получает доступ к зарегистрированным в системе играм и может играть в них один и по сети. Cavoke клиент при запуске игры  запрашивает компонент выбранной игры с сервера, загружает и запускает его на компьютере игрока, предоставляя им доступ к игровому опыту.

<!-- TODO -->

## Подмодули
- [client](/client) – Десктопный клиент на Qt
- [server](/server) – Сервер на Drogon, запускающий логику игр
- [games](/games) – Реализованные игры
