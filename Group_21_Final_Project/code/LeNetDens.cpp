#include "LeNetDens.h"

void Dens::dens_run() {
 if (rst.read()) {
  data_out.write(-1);

  kernel_count = 0;
  clock_dens = 0;
  stage = 0;

  for (int i = 0; i < 256; i++)
   data[i] = 0;
 }
 else if (data_in.read() != -1 && data_size > 0) {
  if (clock_dens == 0)
   //if (clock_dens <= 500)
    //cout << ">> Dens clock = " << clock_dens << endl;

   switch (data_size)
   {
   case 256:
    kernel_count = 120;
    break;
   case 120:
    kernel_count = 84;
    break;
   case 84:
    kernel_count = 10;
    break;
   }

  if (clock_dens < data_size) {
   data[clock_dens] = data_in.read();

   //cout << "data[ " << clock_dens << " ] = " << data[clock_dens] << endl;
  }
  else if (stage < kernel_count) {
   if ((clock_dens >= data_size * (1 + stage) + stage) && (clock_dens <= data_size * (2 + stage) + stage)) {

    if (clock_dens < data_size * (1 + stage) + stage + data_size) {
     //cout << "kernel[ " << clock_dens - (data_size * (1 + stage) + stage) << " ] = " << data_in.read() << endl;
     temp_sum += data_in.read() * data[clock_dens - (data_size * (1 + stage) + stage)];
    }

    if (clock_dens == data_size * (1 + stage) + stage + data_size) {
     //cout << "bias = " << data_in.read() << endl;

     //cout << "current stage = " << stage << endl;

     temp_sum += data_in.read();

     if (data_size != 84)
      temp_sum >= 0 ? temp_sum : temp_sum = 0;

     data_out.write(temp_sum);

     temp_sum = 0;

     stage++;
    }
    else
     data_out.write(-1);
   }
  }

  clock_dens++;
 }
}
