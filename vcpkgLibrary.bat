
cd ../vcpkg

vcpkg integrate remove
vcpkg integrate install

vcpkg upgrade --no-dry-run

vcpkg install libmysql:x64-windows --recurse