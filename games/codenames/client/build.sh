#!/usr/bin/env bash
mkdir -p .build/client
cp -r codenames-client/app.qml .build/client/app.qml
cp -r codenames-client/src .build/client/src
cd .build
zip -r ../client.zip client
cd ..
rm -r .build
