<a href="https://github.com/cavoke-project/cavoke">
  <img align="left" height="80px" src="https://avatars.githubusercontent.com/u/52053547?s=200&v=4">
</a>
<h1 style="display: inline;">
  Cavoke
</h1>

<p>Платформа для создания и проведения сетевых пошаговых настольных игр</p>

[![GitHub Workflow Status](https://img.shields.io/github/workflow/status/cavoke-project/cavoke/app?style=flat-square)](https://github.com/cavoke-project/cavoke/actions/workflows/app.yml)
[![GitHub top language](https://img.shields.io/github/languages/top/cavoke-project/cavoke?logo=github&style=flat-square)](https://github.com/cavoke-project/cavoke)
[![license](https://img.shields.io/github/license/cavoke-project/cavoke?style=flat-square)](./LICENSE)
[![Gitpod Ready-to-Code](https://img.shields.io/badge/Gitpod-Ready--to--Code-orange?logo=gitpod&style=flat-square)](https://gitpod.io/#https://github.com/cavoke-project/cavoke)

<!-- TODO: add video -->

<!-- TODO: add english -->

## Что это
Cavoke – платформа для создания и проведения сетевых пошаговых настольных игр: позволяет разработчику не думать о создании комнат, подключении игроков, передаче данных от сервера к клиентам. Состоит из сервера и игрового клиента (отвечает за запуск интерфейсов игр). 

Разработчик игры регистрирует свой игровой продукт на нашей платформе, предоставляет компоненты с серверной и игровой логикой. Игрок, посредством приложения Cavoke клиента, получает доступ к зарегистрированным в системе играм и может играть в них один и по сети. Cavoke клиент при запуске игры  запрашивает компонент выбранной игры с сервера, загружает и запускает его на компьютере игрока, предоставляя им доступ к игровому опыту.

<!-- TODO -->

## Подмодули
- [client](/client) – Десктопный клиент на Qt
- [server](/server) – Сервер на Drogon, запускающий логику игр
- [games](/games) – Реализованные игры
