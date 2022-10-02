char * pReadInHiddenFile(FILE* pHiddenFile, char fileName) {
    fseek(pHiddenFile, 0L, SEEK_END);
    int hiddenFileSize = ftell(pHiddenFile);
    //printf("SIZE: %d\n", hiddenFileSize);
    waveDataWithHiddenFile = malloc() //Storleken av vår egna heder beror på filnamnets längd? Hur gör vi?

    return 0;
}
