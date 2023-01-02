static char *MakeEnglishOriginalTownName(char *buf, const char *last, uint32 seed)
{
    char *orig = buf;

    /* optional first segment */
    int i = SeedChanceBias(0, lengthof(_name_original_english_1), seed, 50);
    if (i >= 0)
        buf = strecpy(buf, _name_original_english_1[i], last);

    /* mandatory middle segments */
    buf = strecpy(buf, _name_original_english_2[SeedChance(4, lengthof(_name_original_english_2), seed)], last);
    buf = strecpy(buf, _name_original_english_3[SeedChance(7, lengthof(_name_original_english_3), seed)], last);
    buf = strecpy(buf, _name_original_english_4[SeedChance(10, lengthof(_name_original_english_4), seed)], last);
    buf = strecpy(buf, _name_original_english_5[SeedChance(13, lengthof(_name_original_english_5), seed)], last);

    /* optional last segment */
    i = SeedChanceBias(15, lengthof(_name_original_english_6), seed, 60);
    if (i >= 0)
        buf = strecpy(buf, _name_original_english_6[i], last);

    /* Ce, Ci => Ke, Ki */
    if (orig[0] == 'C' && (orig[1] == 'e' || orig[1] == 'i'))
    {
        orig[0] = 'K';
    }

    assert(buf - orig >= 4);
    ReplaceEnglishWords(orig, true);

    return buf;
}