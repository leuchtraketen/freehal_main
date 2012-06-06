#include <stdio.h>
#include <string.h>
#include "../db/md5-db.cpp"

int main() {
    char* test = "Hallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der ElbeHallo an der Elbe";
    printf("test:  %s\n\n", test);
    printf("test:  %s\n\n", to_md5(test));
    printf("test:  %s\n\n", from_md5(to_md5(test)));
    printf("test:  %s\n\n\n", from_md5(test));
    test = "Hallo an der Donau";
    printf("test:  %s\n\n", test);
    printf("test:  %s\n\n", to_md5(test));
    printf("test:  %s\n\n", from_md5(to_md5(test)));
    printf("test:  %s\n\n\n", from_md5(test));

    const char* hex = s_show_in_hex((unsigned char*)test, strlen(test));
    unsigned long buflen;
    unsigned char buf[20001];
    s_show_in_bin(hex, &buflen, buf);
printf("%s\n", buf);


    printf("test:  %s\n\n\n", to_md5(from_md5("789c7d524baedb300c5cd7a7e809a20ff5f3839073b4dd08b444c6011cdb701cf4e5f6559c047eeda2040448430e392219f93c503a8f3c1d9befd5e288173a729f4cb22d042f21997a6492516cae67d46d1e262ce9774f639a17bad2b88a97eb829f69ecee2b5d8f563e2c8a2fd033e644232db85249c3947118ee6ff26d198efdbace1f42f042d4e37018697ddf13aea99f2e94f2e92c36e12f1d3d8e65a0258a07ff99294ff33d3d626aeaf87919d2f57c1a71bd2df50b2883eeb2079f8365cc5d2b0d6727b90353206b764ac990c96bcc1273cb062c66b6de6a3205ad6c1c7ad0521500ab34065008aa736d711e5d96b22d50244b0cad93da29ebc1113a6b257a65804dd7b054400e8c521014b63a77c86cc916998dd18e6df02e18c545076d59b92a2314b2081a1458eb1a42ce5025714056b50cb30fbe4a47554c715454a8c59caf740e0aba36170b5c33da561594ba3487268a7fda12c597658875aab7617d7573732cc4cfe70efd675b76ffce99e6ba2f1bd64de731fff8f9eb302f53143bde7cfb6b721b4becd5a378cb6a9a3f0f11d123")));

}
