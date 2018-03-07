package org.sh.netfinca;

import java.io.IOException;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.web.client.RestTemplate;

import gnu.io.CommPortIdentifier;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

public class WorkCompagnion {

	public static void main(String[] args) throws InterruptedException, PortInUseException {
		new WorkCompagnion().start();
	}

	private String message = "greenflash";
	private SerialPort port;

	private void start() throws InterruptedException, PortInUseException {
		List<CommPortIdentifier> ports = Collections.list(CommPortIdentifier.getPortIdentifiers());
		ports.stream().map(p -> p.getName()).forEach(n -> System.out.println(n));
		CommPortIdentifier portId = ports.stream().filter(p -> p.getName().equals("/dev/ttyUSB0"))
				.findFirst().get();
		port = (SerialPort) portId.open("tutu", 9600);
		Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
			@Override
			public void run() {
				port.removeEventListener();
				port.close();
			}
		}));
		send();
		while (true) {
			TimeUnit.SECONDS.sleep(30);
			String newMessage = read();
			if (!message.equals(newMessage) && newMessage != null) {
				message = newMessage;
				send();
			}
		}
	}

	private String read() {
		try {
			Map<String, Object> params = new HashMap<>();
			params.put("key", "kepkins-status");
			HttpHeaders headers = new HttpHeaders();
			headers.set("security", "gXTcV5QwHf2wWGGyS7Da260DO1Z23mGC");
			return new RestTemplate().exchange("http://foo.shionn.org/fbq/data/{key}",
					HttpMethod.GET, new HttpEntity<String>(null, headers), String.class, params)
					.getBody();
		} catch (RuntimeException e) {
			return "none";
		}
	}

	private void send() {
		byte[] datas = (message).getBytes(Charset.forName("US-ASCII"));
		try {
			port.setSerialPortParams(9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,
					SerialPort.PARITY_NONE);
			System.out.println(message);
			try (OutputStream os = port.getOutputStream()) {
				os.write(datas);
			}
		} catch (UnsupportedCommOperationException | IOException e) {
			e.printStackTrace();
		}
	}

}
