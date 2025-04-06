using System;
using System.Net.Sockets;

namespace WinFormsApp1
{
	public partial class Form1 : Form
	{

		const string ServerIp = "192.168.200.184";
		const int ServerPort = 8080;
		private MemoryStream buffer = new MemoryStream();
		public Form1()
		{
			InitializeComponent();
			ConnectAndWait();
		}
		TcpClient client = new TcpClient(ServerIp, ServerPort);
		NetworkStream stream;
		byte[] response = new byte[2] { 0xAB, 0xCD };
		private void ConnectAndWait()
		{
			Task.Run(() =>
			{
				try
				{
					stream = client.GetStream();
					
					while (client.Connected)
					{
						byte[] recvBuffer = new byte[2048];

						while (true)
						{
							int bytesRead = stream.Read(recvBuffer, 0, recvBuffer.Length);
							if (bytesRead <= 0) break;

							buffer.Write(recvBuffer, 0, bytesRead);
							ProcessBuffer();
							Thread.Sleep(1000 / 4);
						}
					}
				}
				catch (Exception ex)
				{
					MessageBox.Show($"Error: {ex.Message}");
				}
			});
		}

		static bool IsJpegEnd(MemoryStream stream)
		{
			byte[] data = stream.ToArray();
			if (data.Length < 2) return false;

			return data[^2] == 0xFF && data[^1] == 0xD9;
		}

		private void ProcessBuffer()
		{
			byte[] data = buffer.ToArray();
			int start = FindMarker(data, 0xFFD8); // JPEG SOI
			int end = FindMarker(data, 0xFFD9);   // JPEG EOI

			while (start != -1 && end != -1 && end > start)
			{
				int length = end - start + 2; // include EOI marker
				byte[] jpegImage = new byte[length];
				Array.Copy(data, start, jpegImage, 0, length);

				// Display image
				Invoke(() =>
				{
					using MemoryStream ms = new MemoryStream(jpegImage);
					pictureBox1.Image = Image.FromStream(ms);
					stream.Write(response, 0, response.Length);
				});

				// Remove used bytes from buffer
				byte[] remaining = new byte[data.Length - (start + length)];
				Array.Copy(data, start + length, remaining, 0, remaining.Length);
				buffer = new MemoryStream();
				buffer.Write(remaining, 0, remaining.Length);
				data = buffer.ToArray();

				start = FindMarker(data, 0xFFD8);
				end = FindMarker(data, 0xFFD9);
			}
		}

		private int FindMarker(byte[] data, int marker)
		{
			for (int i = 0; i < data.Length - 1; i++)
			{
				if (data[i] == 0xFF && data[i + 1] == (marker & 0xFF))
				{
					return i;
				}
			}
			return -1;
		}
		private void pictureBox1_Click(object sender, EventArgs e)
		{

		}
	}
}
