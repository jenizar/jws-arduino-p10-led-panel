#include <DMD.h>
#include <TimerOne.h>
#include <Wire.h>
#include <Sodaq_DS3231.h>
#include "PrayerTimes.h"
#include <SystemFont5x7.h>
#include <Arial_black_16.h>
#include <SPI.h>
//char weekDay[][7] = {"KAMIS", "JUM'AT", "SABTU", "MINGGU","SENIN","SELASA","RABU"};
char weekDay[][7] = {"MINGGU","SENIN","SELASA","RABU","KAMIS", "JUM'AT", "SABTU"};
int langkah;
int lama_tampilnya;



/*
 * PIN YANG DISAMBUNG
 * Arduino              P-10
 * -D6          ->      -A
 * -D7          ->      -B
 * -D8          ->      -SCLK
 * -D9          ->      -OE
 * -D11         ->      -DATA
 * -D13         ->      -CLK
 * -GND         ->      -GND
 */

DMD dmd(2, 1); //untuk mengatur jumlah panel

double times[sizeof(TimeName) / sizeof(char*)];

void ScanDMD()
{
  dmd.scanDisplayBySPI();
}






void setup()
{

  pinMode(5, OUTPUT);//----pin 5 untuk alarm---hubungkan dengan buzzer

  langkah = 0;
  rtc.begin();
  Wire.begin();
  Timer1.initialize( 500 );
  Timer1.attachInterrupt( ScanDMD );
  dmd.clearScreen( true );




}
int menite = 1; ///waktu iqomah lamanya
void loop()
{

  DateTime now = rtc.now();
  String Teks;

  double lintang = -5.450000;  //Latitude
  double bujur = 105.266670;   //Longitude
  int Z = 7 ;                   //Zona Waktu


  int tgl = now.date();         //Tanggal
  int bln = now.month();        //Bulan
  int thn = now.year();         //Tahun
  int hours, minutes;
  long mulai = millis();

  set_calc_method(ISNA);        //Methode perhitungan mengikuti ISNA
  set_asr_method(Shafii);       //Pendapat Imam Syafi'i
  set_high_lats_adjust_method(AngleBased);
  set_fajr_angle(20);           //Sudut Waktu Subuh
  set_isha_angle(18);           ////Sudut Waktu Isya
  get_prayer_times(thn, bln, tgl, lintang, bujur, Z, times);

  dmd.clearScreen( true );


  dmd.selectFont(SystemFont5x7);



  // Tampilkan jam dan tanggal selama 10 detik
  langkah = 0;

  while (millis() - mulai < 10000) {
    langkah = langkah + 1;
    now = rtc.now();
    String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(7, 0, sjam, pjg, 0);


    if (langkah < 20) {
      dmd.drawString(13, 9, (weekDay[now.dayOfWeek()]), 7, 0);                                    //tampilan hari

    } else if (langkah < 30) {
      String hariku = Konversi(now.date()) + "/" + Konversi(now.month()) + "/" + Konversi(now.year()) + " "; //tampilan tanggal
      int dowo = hariku.length() + 1;
      char detikanku[dowo];
      hariku.toCharArray(detikanku, dowo);
      dmd.drawString(2, 9, detikanku, dowo, 0);

    }
    delay(200);
  }

  //----------------------------------------------====kode waktu adzan====------------------------
  //jam adzan Subuh----------------------------------------------------------
  dmd.clearScreen( true );
  while (millis() - mulai < 11000) {
    ///////////////////////
    now = rtc.now();
    String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(7, 4, sjam, pjg, 0);

    /////////////////////

    get_float_time_parts(times[0], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);
      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 9, "Subuh", 5, 0); //koordinat tampilan
      delay(150000);

      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);



          }
        }
      }
    }
  }





  //----------------------------------------------------------------------

  //jam adzan Dzuhur----------------------------------------------------------
  dmd.clearScreen( true );
  while (millis() - mulai < 20000) {
    ///////////////////////
    now = rtc.now();
    String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(7, 4, sjam, pjg, 0);

    /////////////////////
    get_float_time_parts(times[2], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);
      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(15, 9, "Sholat", 6, 0); //koordinat tampilan
      delay(150000);




      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);

          }
        }
      }
    }
  }






  //----------------------------------------------------------------------

  //jam adzan Asar----------------------------------------------------------
  dmd.clearScreen( true );
  while (millis() - mulai < 31000) {
    ///////////////////////
    now = rtc.now();
    String hariku = Konversi(now.date()) + "/" + Konversi(now.month()) + "/" + Konversi(now.year()) + " "; //tampilan tanggal
    int dowo = hariku.length() + 1;
    char detikanku[dowo];
    hariku.toCharArray(detikanku, dowo);
    dmd.drawString(2, 4, detikanku, dowo, 0);

    /////////////////////
    get_float_time_parts(times[3], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);
      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(19, 9, "Asar", 4, 0); //koordinat tampilan
      delay(150000);




      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);

          }
        }
      }
    }
  }






  //----------------------------------------------------------------------

  //jam adzan Maghrib----------------------------------------------------------
  dmd.clearScreen( true );
  while (millis() - mulai < 40000) {
    ///////////////////////
    now = rtc.now();
    String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);
    dmd.drawString(7, 4, sjam, pjg, 0);

    /////////////////////
    get_float_time_parts(times[4], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);
      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(15, 9, "Maghrib", 7, 0); //koordinat tampilan
      delay(150000);




      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);

          }
        }
      }
    }
  }









  //jam adzan isya----------------------------------------------------------
  dmd.clearScreen( true );
  while (millis() - mulai < 50000) {
    ///////////////////////
    now = rtc.now();
    String hariku = Konversi(now.date()) + "/" + Konversi(now.month()) + "/" + Konversi(now.year()) + " "; //tampilan tanggal
    int dowo = hariku.length() + 1;
    char detikanku[dowo];
    hariku.toCharArray(detikanku, dowo);
    dmd.drawString(2, 4, detikanku, dowo, 0);

    /////////////////////
    get_float_time_parts(times[6], hours, minutes);
    if ((now.hour() == hours && now.minute() == minutes) || (now.hour() == hours && now.minute() == minutes + 1) || (now.hour() == hours && now.minute() == minutes + 2)
        || (now.hour() == hours && now.minute() == minutes + 3)
        || (now.hour() == hours && now.minute() == minutes + 4)
        || (now.hour() == hours && now.minute() == minutes + 5)) {

      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);
      digitalWrite(5, HIGH);//alarm sholat
      delay(5000);
      digitalWrite(5, LOW);//alarm sholat
      delay(1000);

      dmd.clearScreen( true );
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(17, 0, "Adzan ", 5, 0); //koordinat tampilan
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(15, 9, "Isya", 4, 0); //koordinat tampilan
      delay(150000);




      //iqomah----------------------------------
      int langkah;
      int waktu_iqomah;
      int panjang_iqomah;
      int iqomah;
      dmd.clearScreen( true );


      dmd.selectFont(SystemFont5x7);
      dmd.clearScreen( true );
      dmd.drawString(15, 0, "IQOMAH", 6, 0); //koordinat tampilan
      int tampil;
      int detik = 60, menit = 10;
      for (detik = 60; detik >= 0; detik--) {
        delay(1000);
        String iqomah = Konversi(menit) + ":" + Konversi(detik);
        int panjang_iqomah = iqomah.length() + 1;
        char waktu_iqomah[panjang_iqomah];
        iqomah.toCharArray(waktu_iqomah, panjang_iqomah);
        dmd.drawString(18, 9, waktu_iqomah, panjang_iqomah, 0);
        if (detik <= 0) {
          detik = 60;
          menit = menit - 1;
        } if (menit <= 0 && detik <= 1) {
          for (tampil = 0; tampil < 300 ; tampil++) {
            menit = 0;
            detik = 0;
            dmd.clearScreen( true );

            dmd.drawString(15, 0, "SHOLAT", 6, 0); //koordinat tampilan
            ///////////////////////
            now = rtc.now();
            String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute()) + ":" + Konversi(now.second()); //tampilan jam
            int pjg = xjam.length() + 1;
            char sjam[pjg];
            xjam.toCharArray(sjam, pjg);
            dmd.drawString(7, 9, sjam, pjg, 0);

            /////////////////////
            delay (1000);

          }
        }
      }
    }
  }





  //----------------------------------------------------------------------


  //====================================runing teks======================

  dmd.clearScreen(true);

  // Tampilkan Waktu Subuh
  int kecepatan;
  kecepatan = 20;                                   //kecepatan runing teks

  get_float_time_parts(times[0], hours, minutes);
  Teks = "SUBUH " + Konversi(hours) + ":" + Konversi(minutes);

  int pj = Teks.length() + 1;
  char tampil[pj];
  Teks.toCharArray(tampil, pj);
  dmd.drawMarquee(tampil, pj, 63, 4);

  long timer = millis();
  boolean ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();

    }

  }

  //Tampilkan Waktu Dhuhur
  get_float_time_parts(times[2], hours, minutes);
  int suwene;
  int suwene_2;
  suwene = minutes;
  suwene_2 = hours;
  int sholatnya = 0;

  Teks = "DZUHUR " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();

    }


  }

  //Tampilkan Waktu Ashar
  get_float_time_parts(times[3], hours, minutes);
  Teks = "ASHAR " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
    }
  }

  //Tampilkan Waktu Magrib
  get_float_time_parts(times[4], hours, minutes);
  Teks = "MAGRIB " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
    }
  }
  //Tampilkan Waktu Isya
  get_float_time_parts(times[6], hours, minutes);
  Teks = "ISYA  " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
    }
  }

  //Tampilkan Waktu Imsak, 10 Menit sebelum waktu subuh

  get_float_time_parts(times[0], hours, minutes);
  if (minutes < 11)
  {
    minutes = 60 - minutes;
    hours --;
  }
  else {
    minutes = minutes - 10 ;
  }

  Teks = "IMSAK " + Konversi(hours) + ":" + Konversi(minutes);

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
    }
  }

  //Tampilkan PENGUMUMAN


  Teks = " MATIKAN HP SAAT BERADA DI DALAM MASJID ";

  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.drawMarquee(tampil, pj, 63, 4);
  timer = millis();
  ret = false;
  while (!ret) {
    if ((timer + kecepatan) < millis()) {
      ret = dmd.stepMarquee(-1, 0);
      timer = millis();
    }
  }




  //tampilkan tulisan diam-statis-nama masjid 5 detik

  Teks = "MASJID";                    //baris atas
  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);
  dmd.selectFont(SystemFont5x7);
  dmd.drawString(16, 0, tampil, pj, 0); //koordinat tampilan


  Teks = "AL-IKHLAS";                   //baris kedua
  pj = Teks.length() + 1;
  tampil[pj];
  Teks.toCharArray(tampil, pj);

  dmd.selectFont(SystemFont5x7);
  dmd.drawString(5, 9, tampil, pj, 0); //koordinat tampilan
  delay (5000);
  dmd.clearScreen( true );

  //-----------------------------------------tampilkan tulisan waktu sholat statis--------------------------------------------------------------------

  dmd.clearScreen( true );


  int tunda;
  tunda = 0;

  while (millis() - mulai < 100000) {
    tunda = tunda + 1;
    now = rtc.now();
    int tgl = now.date();         //Tanggal
    int bln = now.month();        //Bulan
    int thn = now.year();         //Tahun
    int hours, minutes;
    long mulai = millis();
    String xjam = Konversi(now.hour()) + ":" + Konversi(now.minute())  ; //tampilan jam dan menit
    int pjg = xjam.length() + 1;
    char sjam[pjg];
    xjam.toCharArray(sjam, pjg);

    dmd.drawString(0, 0, sjam, pjg, 0);//tampilan jam dan menit

    String det = Konversi(now.second()) + " ";  //detik
    int pjgn = det.length() + 1;
    char sjamm[pjgn];
    det.toCharArray(sjamm, pjgn);
    dmd.drawLine(  31,  0, 30, 150, GRAPHICS_NORMAL );
    dmd.drawString(10, 9, sjamm, pjgn, 0);     //tampilan detik

    delay(1000);


    if (tunda < 3) {

      get_float_time_parts(times[0], hours, minutes);
      Teks = "SUBUH   ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);              //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan

    } else if (tunda < 6) {

      get_float_time_parts(times[2], hours, minutes);
      Teks = "ZUHUR   ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);             //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan

    } else if (tunda < 9) {

      get_float_time_parts(times[3], hours, minutes);
      Teks = "ASAR    ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);               //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan
    }  else if (tunda < 12) {

      get_float_time_parts(times[4], hours, minutes);
      Teks = "MGRIB    ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);               //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan
    }  else if (tunda < 15) {

      get_float_time_parts(times[6], hours, minutes);

      Teks = "ISYA     ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);               //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan







    }  else if (tunda < 18) {

      get_float_time_parts(times[0], hours, minutes);
      if (minutes < 11)
      {
        minutes = 60 - minutes;
        hours --;
      }
      else {
        minutes = minutes - 10 ;
      }
      Teks = "IMSAK   ";                    //baris atas
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);
      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 0, tampil, pj, 0); //koordinat tampilan

      Teks = Konversi(hours) + ":" + Konversi(minutes);               //baris kedua
      pj = Teks.length() + 1;
      tampil[pj];
      Teks.toCharArray(tampil, pj);

      dmd.selectFont(SystemFont5x7);
      dmd.drawString(34, 9, tampil, pj, 0); //koordinat tampilan
      delay(1000);
      tunda = 0;
    }
  }





  dmd.clearScreen( true );

}

String Konversi(int sInput) {
  if (sInput < 10)
  {
    return"0" + String(sInput);
  }
  else
  {
    return String(sInput);
  }
}
