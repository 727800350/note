import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;

import java.io.FileNotFoundException;
import java.io.IOException;

public class reader_kv{
	public static void usage(){
		System.err.println("usage: prog data.kv");
	}

	public static int process(String k, byte[]v){
		System.out.println(k + "\t" + v.length);
		return 0;
	}

	public static void main(String[] args){
		if(args.length != 1){
			usage();
			return;
		}
		File file = new File(args[0]);
		try{
			InputStream in = new FileInputStream(file);
			byte[] kl = new byte[4];
			byte[] vl = new byte[4];
			while(in.read(kl) != -1){
				byte[] k = new byte[byte2int(kl)];
				in.read(k);
				in.read(vl);
				byte[] v = new byte[byte2int(vl)];
				in.read(v);
				int ret = process(new String(k), v);
				if(ret != 0){
					System.err.println("process " + new String(k) + " error");
				}
			}
			in.close();
		}
		catch(FileNotFoundException e){
			e.printStackTrace();
		}
		catch(IOException e){
			e.printStackTrace();
		}
	}

	public static int byte2int(byte[] res){
		return (res[0] & 0xff) | ((res[1] << 8) & 0xff00) | ((res[2] << 24) >> 8) | (res[3] << 24);
	}
}

