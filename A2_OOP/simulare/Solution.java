import java.io.*;
import java.util.*;

enum Descriptor
{
    // public final static int length=3;

    ADD("ADD"),

    Q01("Q01"),

    Q02("Q02");

    private String descriptor;
    private Descriptor(String descriptor)
    {
        this.descriptor=descriptor;
    }

    public static Descriptor getDescriptor(String descriptor)
    {
        switch(descriptor)
        {
            case "ADD":
                return ADD;
            case "Q01":
                return Q01;
            case "Q02":
                return Q02;
            default:
                return null;
        }
    }
}

class Path
{
    public List<String> elements;
    public Path(String path)
    {
        this.elements=Arrays.asList(path.split("/"));
    }
}

abstract class Command
{
    public Descriptor descriptor;
    public String parameter;

    public Command(Descriptor descriptor, String parameter)
    {
        this.descriptor=descriptor;
        this.parameter=parameter;
    }

    public abstract void run(Folder root);

    public static Command getCommand(String line)
    {
        Descriptor descriptor=Descriptor.getDescriptor(line.substring(0, 3));
        String command=line.substring(4);
        switch(descriptor)
        {

            case ADD:
                return new CommandADD(command);
            case Q01:
                return new CommandQ01(command);
            case Q02:
                return new CommandQ02(command);
            default:
                System.out.println("UNIMPLEMENTED COMMAND!!!");
                return null;
        }
    }
}

class CommandADD extends Command
{
    public CommandADD(String parameter)
    {
        super(Descriptor.ADD, parameter);
    }

    public void run(Folder root)
    {
        Folder cursor=root;
        Path path=new Path(this.parameter);
        for(int i=0;i<path.elements.size()-1;i++)
        {
            String folderName=path.elements.get(i);
            cursor=cursor.getOrCreateChild(folderName);
        }

        cursor.files.add(path.elements.get(path.elements.size()-1));
    }

}

class CommandQ01 extends Command
{
    public CommandQ01(String parameter)
    {
        super(Descriptor.Q01, parameter);
    }

    public void run(Folder root)
    {
        Folder cursor=root;
        Path path=new Path(this.parameter);
        for(int i=0;i<path.elements.size();i++)
        {
            String folderName=path.elements.get(i);
            cursor=cursor.getChild(folderName);
            if(cursor==null)
            {
                System.out.println("invalid");
                return;
            }
        }

        for(String fileName:cursor.files)
        {
            System.out.println(fileName);
        }
    }
}

class CommandQ02 extends Command
{
    public CommandQ02(String parameter)
    {
        super(Descriptor.Q02, parameter);
    }

    public void run(Folder root)
    {
        Folder cursor=root;
        Path path=new Path(this.parameter);
        for(int i=0;i<path.elements.size()-1;i++)
        {
            String folderName=path.elements.get(i);
            cursor=cursor.getChild(folderName);
            if(cursor==null)
            {
                System.out.println("invalid");
                return;
            }
        }

        String name=path.elements.get(path.elements.size()-1);
        if(cursor.getChild(name)!=null)
        {
            System.out.println("folder");
            return;
        }
        else if(cursor.files.contains(name))
        {
            System.out.println("file");
            return;
        }
        else
        {
            System.out.println("invalid");
            return;
        }
    }
}

class Folder
{
    public String name;
    public Folder parent;
    public List<Folder> children;
    public List<String> files;

    public Folder(String name, Folder parent)
    {
        this.name=name;
        if(parent!=null)
            this.parent=parent;
        else
            this.parent=parent;
        this.children=new ArrayList();
        this.files=new ArrayList();
    }

    public Folder getOrCreateChild(String name)
    {
        for(Folder folder:this.children)
            if(folder.name.equals(name))
                return folder;

        Folder newFolder=new Folder(name, this);
        this.children.add(newFolder);
        return newFolder;
    }

    public Folder getChild(String name)
    {
        for(Folder folder:this.children)
            if(folder.name.equals(name))
                return folder;

        return null;
    }
}

public class Solution {

    public static void main(String[] args) {
        /* Enter your code here. Read input from STDIN. Print output to STDOUT.
        Your class should be named Solution. */

        Scanner scanner = new Scanner(System.in);
        String line;
        Folder root=new Folder("",null);
        line=scanner.nextLine();
        do {
            Command.getCommand(line).run(root);
            line=scanner.nextLine();
        } while (line.length()>3);
        scanner.close();
    }
}
