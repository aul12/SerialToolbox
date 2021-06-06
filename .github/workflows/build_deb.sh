#!/usr/bin/env bash

script=$(realpath $0)
scriptpath=$(dirname $script)
repo_root=${scriptpath}/../..

package_name=$1
package_version=$2
dirname="${package_name}_${package_version}"

# Build
mkdir build
cd build || exit
cmake -DCMAKE_INSTALL_PREFIX=${dirname}/usr/local -DBUILD_TEST=OFF "${repo_root}"
make -j$(nproc) install

# Package config
mkdir -p ${dirname}/DEBIAN
cp "${repo_root}"/.github/workflows/debian_control ${dirname}/DEBIAN/control
sed -i "s/PACKAGE_NAME/$package_name/g" ${dirname}/DEBIAN/control
sed -i "s/PACKAGE_VERSION/$package_version/g" ${dirname}/DEBIAN/control

# Build package
dpkg-deb --build ${dirname}
mv ${dirname}.deb ..
