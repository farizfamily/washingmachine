package ridei.saatnya.online.bluetoothone;

import androidx.appcompat.app.AppCompatActivity;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class MainControl extends AppCompatActivity {

    //Button UpOn, DownOn,
    Button UpOff, Discnt,  ReadBT;
    String address = null;
    String baca="";
    String tulis="";
    private ProgressDialog progress;
    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    //SPP UUID. Look for it
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    Thread workerThread;
    byte[] readBuffer;
    int readBufferPosition;
    int counter;
    volatile boolean stopWorker;
    OutputStream mmOutputStream;
    InputStream mmInputStream;
    EditText myLabel, tekskesana;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_control);
        myLabel = (EditText) findViewById(R.id.teksdarisana);
        tekskesana = (EditText) findViewById(R.id.tekskirimkesana);
        SharedPreferences sharedPreferences = getSharedPreferences("alamat",MODE_PRIVATE);
        String s1 = sharedPreferences.getString("alamat", "");
        if(s1.equals("")){
            Intent intx = new Intent(getApplicationContext(), SelectYourDevice.class);
            //intx.putExtra("barcode", intentData.toString());
            finish();
            startActivity(intx);

        } else {
            address = s1;
            new ConnectBT().execute(); //Call the class to connect
        }

        Intent newint = getIntent();
        //address = newint.getStringExtra(SelectYourDevice.EXTRA_ADDRESS); //receive the address of the bluetooth device

        //UpOn = (Button)findViewById(R.id.up_on);
        //DownOn = (Button)findViewById(R.id.down_on);
        UpOff =(Button)findViewById(R.id.up_off);
        //DownOff=(Button)findViewById(R.id.down_off);
        ReadBT = (Button) findViewById(R.id.readBT);
        Discnt = (Button) findViewById(R.id.disconnect);


        Discnt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Disconnect();
            }
        });

        ReadBT.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                beginListenForData();
            }
        });
        //beginListenForData();

        /*UpOn.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                sendData("1");
            }
        });

        DownOn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                sendData("2");
            }
        });*/

        UpOff.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View v)
            {
                //sendData("3");
                sendData(tekskesana.getText().toString());
            }
        });

        /*DownOff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v)
            {
                sendData("4");
            }
        });*/


    }

    private void sendData(String dataKirim)
    {
        if (btSocket!=null)
        {
            try
            {
                int baca=0;
                btSocket.getOutputStream().write(dataKirim.toString().getBytes());
                beginListenForData();
            }
            catch (IOException e)
            {
                msg("Error");
            }
        }
    }
    private void Disconnect()
    {
        if (btSocket!=null) //If the btSocket is busy
        {
            try
            {
                btSocket.close(); //close connection
            }
            catch (IOException e)
            { msg("Error");}
        }
        finish(); //return to the first layout

    }

    // fast way to call Toast
    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void>  // UI thread
    {
        private boolean ConnectSuccess = true; //if it's here, it's almost connected

        @Override
        protected void onPreExecute()
        {
            progress = ProgressDialog.show(MainControl.this, "Connecting...", "Please wait!!!");  //show a progress dialog
        }

        @Override
        protected Void doInBackground(Void... devices) //while the progress dialog is shown, the connection is done in background
        {
            try
            {
                if (btSocket == null || !isBtConnected)
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice dispositivo = myBluetooth.getRemoteDevice(address);//connects to the device's address and checks if it's available
                    btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                    mmInputStream = btSocket.getInputStream();
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
            return null;
        }
        @Override
        protected void onPostExecute(Void result) //after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);

            if (!ConnectSuccess)
            {
                msg("Connection Failed. Is it a SPP Bluetooth? Try again.");
                finish();
            }
            else
            {
                msg("Connected.");
                isBtConnected = true;
            }
            progress.dismiss();
        }
    }



    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {

            Intent intx = new Intent(getApplicationContext(), SelectYourDevice.class);
            startActivity(intx);
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    //trheat
    void beginListenForData()
    {
        final Handler handler = new Handler();
        final byte delimiter = 10; //This is the ASCII code for a newline character

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];
        workerThread = new Thread(new Runnable()
        {
            public void run()
            {
                while(!Thread.currentThread().isInterrupted() && !stopWorker)
                {
                    try
                    {
                        int bytesAvailable = mmInputStream.available();
                        if(bytesAvailable > 0)
                        {
                            byte[] packetBytes = new byte[bytesAvailable];
                            mmInputStream.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++)
                            {
                                byte b = packetBytes[i];
                                if(b == delimiter)
                                {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;

                                    handler.post(new Runnable()
                                    {
                                        public void run()
                                        {
                                            myLabel.setText(data);
                                        }
                                    });
                                }
                                else
                                {
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    }
                    catch (IOException ex)
                    {
                        stopWorker = true;
                    }
                }
            }
        });

        workerThread.start();
    }
    //thread



}