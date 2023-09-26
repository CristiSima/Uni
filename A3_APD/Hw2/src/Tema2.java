import java.io.FileReader​;
import java.io.FileOutputStream;
import java.util.Scanner;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ForkJoinPool;
import java.util.concurrent.RecursiveTask;

class Tema2
{
	public static long getFileSize(String file_path)
	{
		try {
			// FileReader​ reader=new FileReader​(file_path);
			// return reader.skip(1000000000);
			return Files.size(Paths.get(file_path));
		} catch(Exception e) {

		}
		return -1;
	}


	static int no_threads;
	static long orders_size;
	static long products_size;
	static String input_folder;

	static FileOutputStream order_output;
	static FileOutputStream product_output;

	public static void main(String[] args) {
		input_folder=args[0];
		no_threads=Integer.parseInt(args[1]);

		// System.out.println("input_folder:\t"+input_folder);
		// System.out.println("no_threads:\t"+no_threads);

		orders_size=getFileSize(input_folder+"/orders.txt");
		products_size=getFileSize(input_folder+"/order_products.txt");

		// System.out.println(orders_size);
		// System.out.println(products_size);

		try {
			order_output=new FileOutputStream("orders_out.txt");
			product_output=new FileOutputStream("order_products_out.txt");
		} catch(Exception e) { }


		ForkJoinPool pool=new ForkJoinPool(no_threads);

		for(int i=0;i<no_threads;i++)
			pool.invoke(new OrderWorker(
				input_folder+"/orders.txt",
				( i   *orders_size)/no_threads,
				((i+1)*orders_size)/no_threads
			));

		pool.shutdown();
	}
}

class SplitFileWorker extends RecursiveTask<Void>
{
	String file_path;

	long start_offset;
	long end_offset;
	long pos;

	Scanner scanner;


	public SplitFileWorker(String file_path, long start_offset, long end_offset)
	{
		this.file_path=file_path;
		this.start_offset=start_offset;
		this.end_offset=end_offset;
	}

	Scanner initScanner()
	{
		FileReader​ reader;
		scanner=null;
		String token;

		char[] cbuff=new char[10];
		try {

			reader=new FileReader​(file_path);
			scanner=new Scanner(reader);
			pos=0;

			if(start_offset==0)
				return scanner;

			pos=start_offset;
			reader.skip(start_offset-1);
			reader.read(cbuff, 0, 1);

			while(cbuff[0]!='\n')
			{
				reader.read(cbuff, 0, 1);
				pos++;
			}
		} catch(Exception e) {}

		return scanner;
	}


	String getNext()
	{
		while(pos<end_offset)
			if(scanner.hasNext())
			{
				String token=scanner.next();
				pos+=token.length()+1;

				return token;
			}
			else
				return null;

		return null;
	}


	void process(String line)
	{
		System.out.println(start_offset+"\t"+pos+"\t"+end_offset+"\t"+line);
	}

	@Override
	protected Void compute()
	{
		scanner=initScanner();
		String token;

		while((token=getNext())!=null)
			process(token);

		return null;
	}
}


class OrderWorker extends SplitFileWorker
{
	public OrderWorker(String file_path, long start_offset, long end_offset)
	{
		super(file_path, start_offset, end_offset);
	}

	@Override
	void process(String line)
	{
		String[] parts = line.split(",");
		String command_id = parts[0];
		int item_count = Integer.parseInt(parts[1]);

		if(item_count==0)
			return;

		List<ProductsWorker> tasks = new ArrayList<>();
		ProductsWorker worker;

		for(int i=0;i<Tema2.no_threads;i++)
		{
			worker=new ProductsWorker(
				Tema2.input_folder+"/order_products.txt",
				( i   *Tema2.products_size)/Tema2.no_threads,
				((i+1)*Tema2.products_size)/Tema2.no_threads,
				command_id
			);

			tasks.add(worker);
			worker.fork();
		}

		for (var task : tasks)
			task.join();

		int total_shipped=0;
		for (var task : tasks)
			total_shipped+=task.shipped;

		if(total_shipped!=item_count)
			return;

		String out_line=line+",shipped\n";
		try {
			byte[] vec=out_line.getBytes("UTF-8");

			synchronized (Tema2.order_output)
			{
				Tema2.order_output.write(vec, 0, out_line.length());
			}
		} catch(Exception e) {}
	}
}


class ProductsWorker extends SplitFileWorker
{
	String order_id;

	public int shipped;

	public ProductsWorker(String file_path, long start_offset, long end_offset, String order_id)
	{
		super(file_path, start_offset, end_offset);
		this.order_id=order_id;
		shipped=0;
	}


	void dummy_action()
	{
		int a=0;
		for(int i=0;i<100;i++)
		{
			a=a+1;
		}
	}

	@Override
	void process(String line)
	{
		String[] parts = line.split(",");
		String command_id = parts[0];
		String product_id = parts[1];

		if(!command_id.equals(order_id))
			return;

		dummy_action();
		shipped++;

		String out_line=line+",shipped\n";
		try {
			byte[] vec=out_line.getBytes("UTF-8");

			synchronized (Tema2.product_output)
			{
				Tema2.product_output.write(vec, 0, out_line.length());
			}
		} catch(Exception e) {}
	}
}
