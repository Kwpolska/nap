# Maintainer: Chris “Kwpolska” Warrick <chris@chriswarrick.com>
pkgname=nap
_prname=nap
pkgver=0.1.1
pkgrel=1
pkgdesc='sleep with a progressbar'
arch=('i686' 'x86_64')
url='https://github.com/Kwpolska/nap'
license=('BSD')
depends=()
makedepends=('make')
options=(!emptydirs)
source=("https://github.com/Kwpolska/${_prname}/releases/download/v${pkgver}/${_prname}-${pkgver}.tar.gz")
md5sums=('816cab07350015f1253c492502c4101d')

build() {
  cd "${srcdir}/${_prname}-${pkgver}"
  ./configure --prefix=/usr
  make
}

package() {
  cd "${srcdir}/${_prname}-${pkgver}"
  make DESTDIR="${pkgdir}" install
}

# vim:set ts=2 sw=2 et:
