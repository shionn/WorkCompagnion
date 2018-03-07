package org.sh.netfinca;

public enum Message {
	available("greenflash".getBytes()), compile("redring".getBytes());

	private byte[] data;

	private Message(byte[] data) {
		this.data = data;
	}

	public byte[] getData() {
		return data;
	}

}
