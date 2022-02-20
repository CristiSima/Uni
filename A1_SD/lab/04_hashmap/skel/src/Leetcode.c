bool isAnagram(char * s, char * t)
{
    short s_freq[26];
    short t_freq[26];

    for(int i=0;i<26;i++)
        s_freq[i]=t_freq[i]=0;

    for(int i=0;s[i]!=0;i++)
        s_freq[s[i]-'a']++;

    for(int i=0;t[i]!=0;i++)
        t_freq[t[i]-'a']++;

    for(int i=0;i<26;i++)
        if(s_freq[i]!=t_freq[i])
            return false;

    return true;
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    for(int i=0;i<numsSize;i++)
        for(int j=i+1;j<numsSize;j++)
            if(nums[i]+nums[j]==target)
            {
                int *p=malloc(sizeof(int)*2);
                p[0]=i;
                p[1]=j;
                returnSize[0]=2;
                return p;
            }
    return NULL;
}
