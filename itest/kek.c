int kek() {
    char a[8];
    printf("kek: %p\na(stack): %p\n", &kek, a);
    printf("%p %p %p %p %p %p %p %p %p %p %p\n");
    scanf("%s", a);
    printf("%p %p %p %p %p %p %p %p %p %p %p\n");

}
int main() {kek();}
