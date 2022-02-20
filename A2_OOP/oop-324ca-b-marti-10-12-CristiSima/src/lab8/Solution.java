package lab8;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.Scanner;

public class Solution{
    public static void main(String[] args)
    {
        Scanner s =new Scanner(System.in);
        int numbers=s.nextInt();
        int number;
        HashMap<Integer,Integer> frec=new HashMap();
        ArrayList<Integer> order=new ArrayList();
        for(int i=0;i<numbers;i++)
        {
            number=s.nextInt();
            if(!frec.containsKey(number))
            {
                frec.put(number,1);
                order.add(number);
            }
            else
            {
                frec.put(number,1+frec.get(number));
            }
        }
        // for(HashMap.Entry<Integer,Integer> entry :frec.entrySet())
        // {
        //     System.out.print(entry.getKey()+": "+entry.getValue()+" ");
        // }
        for(int key :order)
        {
            System.out.print(key+": "+frec.get(key)+" ");
        }
        // System.out.println(frec);
    }
}
