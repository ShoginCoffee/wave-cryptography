char * pReadInHiddenFile(FILE* pHiddenFile, char fileName) {
    fseek(pHiddenFile, 0L, SEEK_END);
    int hiddenFileSize = ftell(pHiddenFile);
    //printf("SIZE: %d\n", hiddenFileSize);
    waveDataWithHiddenFile = malloc() //Storleken av v�r egna heder beror p� filnamnets l�ngd? Hur g�r vi?

    return 0;
}
