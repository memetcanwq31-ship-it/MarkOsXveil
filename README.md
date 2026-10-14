# MarkOS

MarkOS, tamamen kendi ekosistemini kurmaya odaklanan, özgür ve halka açık bir işletim sistemi projesidir. Amaç, Linux tabanlı bağımlılıklardan sıyrılan, kendi çekirdeği, kullanıcı alanı, paket sistemi ve dağıtım modeli olan bir sistem kurmaktır.

Bu proje, öğretici ve geliştirme odaklı olarak oluşturulmuştur. Hedef, temel bootloader, çekirdek, init sistemleri ve güvenli kullanıcı alanı katmanlarını adım adım inşa etmektir.

## Amaç

- Linux'a bağımlı olmayan özgür bir dağıtım hedefi
- Kendi çekirdeği ve temel sistem bileşenleri
- C, C++, Assembly ve özgün araç zinciri kullanarak düşük seviyeli geliştirme
- Güvenlik odaklı, doğrulanabilir, modüler yapı
- Açık kaynak ve topluluk katkısına açık bir geliştirme modeli

## Proje Yapısı

- bootloader/ : Sistem önyükleme mantığı
- kernel/ : Çekirdek ve temel sistem bileşenleri
- userspace/ : Uygulama ve servis katmanı
- pkg/ : Paket sistemi ve sürüm yönetimi
- installer/ : Kurulum ve dağıtım aracı
- docs/ : Mimari, tasarım ve kullanım belgeleri
- ci/ : Build ve test otomasyonu

## Geliştirme yaklaşımı

- C/C++ ile düşük seviyeli sistem kodu
- Gerekli durumlarda Assembly kullanımı
- QEMU ve test ortamı ile doğrulama
- Modüler ve taşınabilir kod yapısı
- Açık kaynak lisans altında topluluk katkısı

## Lisans

Bu proje GNU Genel Kamu Lisansı (GPL) v3 altında lisanslanmıştır. Tam metin için LICENSE dosyasına bakınız.

## Not

Bu proje henüz erken geliştirme aşamasındadır. Amaç, güvenli ve sürdürülebilir bir işletim sistemi altyapısını adım adım kurmaktır.

## Katkı

Katkı yapmak isterseniz, önce proje mimarisini ve geliştirme hedeflerini inceleyin. Paylaşım, iyileştirme ve hata raporları memnuniyetle kabul edilir.

## Telif Hakkı

© 2026 MarkOS
