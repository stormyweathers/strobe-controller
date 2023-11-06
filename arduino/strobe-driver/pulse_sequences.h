#ifndef PULSE_SEQUENCES_H
#define PULSE_SEQUENCES_H 

// For the slow dance effect
volatile uint32_t cycle_path[4]={0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff};
volatile uint32_t two_piece[2]={0xFF00FF00,0x00FF00FF};
volatile uint32_t all_strobe[2]={0xFFFFFFFF,0xFFFFFFFF};

volatile uint32_t pulse_5[]={0xFF000000,0xFFFF0000, 0x00FF0000,0x00FFFF00,0x0000FF00};
volatile uint32_t six_step[]={0xFF000000,0x00FF0000, 0x0000FF00,0x80800000,0x80008000, 0x00808000 };




//Primary colors and their complements
const uint32_t R  = 0xFF000000;
const uint32_t G  = 0x00FF0000;
const uint32_t B  = 0x0000FF00;
const uint32_t R_ = 0x00808000;
const uint32_t G_ = 0x80008000;
const uint32_t B_ = 0x80800000;

//Color modes for fan blade:
volatile uint32_t two_tone[]={G,G_};
volatile uint32_t four_tone[]={R,R_,B,B_};
volatile uint32_t five_tone[]={R,R_,G_,B_,B};


// Precomputed pulse codes for sierpinksi walks in RGB simplex space
volatile uint32_t fractal_path_0[3]={0xff000000, 0xff0000, 0x00ff00};
volatile uint32_t fractal_path_1[9]={0xff000000, 0x80800000, 0x80008000, 0x808000, 0x80800000, 0xff0000, 0x808000, 0x00ff00, 0x80008000};
volatile uint32_t fractal_path_2[27]={0xff000000, 0xc0400000, 0xc0004000, 0x80404000, 0xc0400000, 0x80800000, 0x80404000, 0x80008000, 0x40408000, 0x4000c000, 0x40c000, 0x40408000, 0x808000, 0x40804000, 0x80800000, 0x40c00000, 0x40804000, 0xc04000, 0x40c00000, 0xff0000, 0xc04000, 0x808000, 0x40c000, 0x00ff00, 0x4000c000, 0x80008000, 0xc0004000};
volatile uint32_t fractal_path_3[81]={0xff000000, 0xe0200000, 0xe0002000, 0xc0202000, 0xe0200000, 0xc0400000, 0xc0202000, 0xc0004000, 0xa0204000, 0xa0006000, 0x80206000, 0xa0204000, 0x80404000, 0xa0402000, 0xc0400000, 0xa0600000, 0xa0402000, 0x80602000, 0xa0600000, 0x80800000, 0x80602000, 0x80404000, 0x80206000, 0x80008000, 0x60208000, 0x6000a000, 0x4020a000, 0x60208000, 0x40408000, 0x4020a000, 0x4000c000, 0x2020c000, 0x2000e000, 0x20e000, 0x2020c000, 0x40c000, 0x2040a000, 0x40408000, 0x20608000, 0x2040a000, 0x60a000, 0x20608000, 0x808000, 0x20806000, 0x40804000, 0x60802000, 0x80800000, 0x60a00000, 0x60802000, 0x40a02000, 0x60a00000, 0x40c00000, 0x40a02000, 0x40804000, 0x20a04000, 0x20806000, 0xa06000, 0x20a04000, 0xc04000, 0x20c02000, 0x40c00000, 0x20e00000, 0x20c02000, 0xe02000, 0x20e00000, 0xff0000, 0xe02000, 0xc04000, 0xa06000, 0x808000, 0x60a000, 0x40c000, 0x20e000, 0x00ff00, 0x2000e000, 0x4000c000, 0x6000a000, 0x80008000, 0xa0006000, 0xc0004000, 0xe0002000};
volatile uint32_t fractal_path_4[243]={0xff000000, 0xf0100000, 0xf0001000, 0xe0101000, 0xf0100000, 0xe0200000, 0xe0101000, 0xe0002000, 0xd0102000, 0xd0003000, 0xc0103000, 0xd0102000, 0xc0202000, 0xd0201000, 0xe0200000, 0xd0300000, 0xd0201000, 0xc0301000, 0xd0300000, 0xc0400000, 0xc0301000, 0xc0202000, 0xc0103000, 0xc0004000, 0xb0104000, 0xb0005000, 0xa0105000, 0xb0104000, 0xa0204000, 0xa0105000, 0xa0006000, 0x90106000, 0x90007000, 0x80107000, 0x90106000, 0x80206000, 0x90205000, 0xa0204000, 0x90304000, 0x90205000, 0x80305000, 0x90304000, 0x80404000, 0x90403000, 0xa0402000, 0xb0401000, 0xc0400000, 0xb0500000, 0xb0401000, 0xa0501000, 0xb0500000, 0xa0600000, 0xa0501000, 0xa0402000, 0x90502000, 0x90403000, 0x80503000, 0x90502000, 0x80602000, 0x90601000, 0xa0600000, 0x90700000, 0x90601000, 0x80701000, 0x90700000, 0x80800000, 0x80701000, 0x80602000, 0x80503000, 0x80404000, 0x80305000, 0x80206000, 0x80107000, 0x80008000, 0x70108000, 0x70009000, 0x60109000, 0x70108000, 0x60208000, 0x60109000, 0x6000a000, 0x5010a000, 0x5000b000, 0x4010b000, 0x5010a000, 0x4020a000, 0x50209000, 0x60208000, 0x50308000, 0x50209000, 0x40309000, 0x50308000, 0x40408000, 0x40309000, 0x4020a000, 0x4010b000, 0x4000c000, 0x3010c000, 0x3000d000, 0x2010d000, 0x3010c000, 0x2020c000, 0x2010d000, 0x2000e000, 0x1010e000, 0x1000f000, 0x10f000, 0x1010e000, 0x20e000, 0x1020d000, 0x2020c000, 0x1030c000, 0x1020d000, 0x30d000, 0x1030c000, 0x40c000, 0x1040b000, 0x2040a000, 0x30409000, 0x40408000, 0x30508000, 0x30409000, 0x20509000, 0x30508000, 0x20608000, 0x20509000, 0x2040a000, 0x1050a000, 0x1040b000, 0x50b000, 0x1050a000, 0x60a000, 0x10609000, 0x20608000, 0x10708000, 0x10609000, 0x709000, 0x10708000, 0x808000, 0x10807000, 0x20806000, 0x30805000, 0x40804000, 0x50803000, 0x60802000, 0x70801000, 0x80800000, 0x70900000, 0x70801000, 0x60901000, 0x70900000, 0x60a00000, 0x60901000, 0x60802000, 0x50902000, 0x50803000, 0x40903000, 0x50902000, 0x40a02000, 0x50a01000, 0x60a00000, 0x50b00000, 0x50a01000, 0x40b01000, 0x50b00000, 0x40c00000, 0x40b01000, 0x40a02000, 0x40903000, 0x40804000, 0x30904000, 0x30805000, 0x20905000, 0x30904000, 0x20a04000, 0x20905000, 0x20806000, 0x10906000, 0x10807000, 0x907000, 0x10906000, 0xa06000, 0x10a05000, 0x20a04000, 0x10b04000, 0x10a05000, 0xb05000, 0x10b04000, 0xc04000, 0x10c03000, 0x20c02000, 0x30c01000, 0x40c00000, 0x30d00000, 0x30c01000, 0x20d01000, 0x30d00000, 0x20e00000, 0x20d01000, 0x20c02000, 0x10d02000, 0x10c03000, 0xd03000, 0x10d02000, 0xe02000, 0x10e01000, 0x20e00000, 0x10f00000, 0x10e01000, 0xf01000, 0x10f00000, 0xff0000, 0xf01000, 0xe02000, 0xd03000, 0xc04000, 0xb05000, 0xa06000, 0x907000, 0x808000, 0x709000, 0x60a000, 0x50b000, 0x40c000, 0x30d000, 0x20e000, 0x10f000, 0x00ff00, 0x1000f000, 0x2000e000, 0x3000d000, 0x4000c000, 0x5000b000, 0x6000a000, 0x70009000, 0x80008000, 0x90007000, 0xa0006000, 0xb0005000, 0xc0004000, 0xd0003000, 0xe0002000, 0xf0001000};
volatile uint32_t fractal_path_5[729]={0xff000000, 0xf8080000, 0xf8000800, 0xf0080800, 0xf8080000, 0xf0100000, 0xf0080800, 0xf0001000, 0xe8081000, 0xe8001800, 0xe0081800, 0xe8081000, 0xe0101000, 0xe8100800, 0xf0100000, 0xe8180000, 0xe8100800, 0xe0180800, 0xe8180000, 0xe0200000, 0xe0180800, 0xe0101000, 0xe0081800, 0xe0002000, 0xd8082000, 0xd8002800, 0xd0082800, 0xd8082000, 0xd0102000, 0xd0082800, 0xd0003000, 0xc8083000, 0xc8003800, 0xc0083800, 0xc8083000, 0xc0103000, 0xc8102800, 0xd0102000, 0xc8182000, 0xc8102800, 0xc0182800, 0xc8182000, 0xc0202000, 0xc8201800, 0xd0201000, 0xd8200800, 0xe0200000, 0xd8280000, 0xd8200800, 0xd0280800, 0xd8280000, 0xd0300000, 0xd0280800, 0xd0201000, 0xc8281000, 0xc8201800, 0xc0281800, 0xc8281000, 0xc0301000, 0xc8300800, 0xd0300000, 0xc8380000, 0xc8300800, 0xc0380800, 0xc8380000, 0xc0400000, 0xc0380800, 0xc0301000, 0xc0281800, 0xc0202000, 0xc0182800, 0xc0103000, 0xc0083800, 0xc0004000, 0xb8084000, 0xb8004800, 0xb0084800, 0xb8084000, 0xb0104000, 0xb0084800, 0xb0005000, 0xa8085000, 0xa8005800, 0xa0085800, 0xa8085000, 0xa0105000, 0xa8104800, 0xb0104000, 0xa8184000, 0xa8104800, 0xa0184800, 0xa8184000, 0xa0204000, 0xa0184800, 0xa0105000, 0xa0085800, 0xa0006000, 0x98086000, 0x98006800, 0x90086800, 0x98086000, 0x90106000, 0x90086800, 0x90007000, 0x88087000, 0x88007800, 0x80087800, 0x88087000, 0x80107000, 0x88106800, 0x90106000, 0x88186000, 0x88106800, 0x80186800, 0x88186000, 0x80206000, 0x88205800, 0x90205000, 0x98204800, 0xa0204000, 0x98284000, 0x98204800, 0x90284800, 0x98284000, 0x90304000, 0x90284800, 0x90205000, 0x88285000, 0x88205800, 0x80285800, 0x88285000, 0x80305000, 0x88304800, 0x90304000, 0x88384000, 0x88304800, 0x80384800, 0x88384000, 0x80404000, 0x88403800, 0x90403000, 0x98402800, 0xa0402000, 0xa8401800, 0xb0401000, 0xb8400800, 0xc0400000, 0xb8480000, 0xb8400800, 0xb0480800, 0xb8480000, 0xb0500000, 0xb0480800, 0xb0401000, 0xa8481000, 0xa8401800, 0xa0481800, 0xa8481000, 0xa0501000, 0xa8500800, 0xb0500000, 0xa8580000, 0xa8500800, 0xa0580800, 0xa8580000, 0xa0600000, 0xa0580800, 0xa0501000, 0xa0481800, 0xa0402000, 0x98482000, 0x98402800, 0x90482800, 0x98482000, 0x90502000, 0x90482800, 0x90403000, 0x88483000, 0x88403800, 0x80483800, 0x88483000, 0x80503000, 0x88502800, 0x90502000, 0x88582000, 0x88502800, 0x80582800, 0x88582000, 0x80602000, 0x88601800, 0x90601000, 0x98600800, 0xa0600000, 0x98680000, 0x98600800, 0x90680800, 0x98680000, 0x90700000, 0x90680800, 0x90601000, 0x88681000, 0x88601800, 0x80681800, 0x88681000, 0x80701000, 0x88700800, 0x90700000, 0x88780000, 0x88700800, 0x80780800, 0x88780000, 0x80800000, 0x80780800, 0x80701000, 0x80681800, 0x80602000, 0x80582800, 0x80503000, 0x80483800, 0x80404000, 0x80384800, 0x80305000, 0x80285800, 0x80206000, 0x80186800, 0x80107000, 0x80087800, 0x80008000, 0x78088000, 0x78008800, 0x70088800, 0x78088000, 0x70108000, 0x70088800, 0x70009000, 0x68089000, 0x68009800, 0x60089800, 0x68089000, 0x60109000, 0x68108800, 0x70108000, 0x68188000, 0x68108800, 0x60188800, 0x68188000, 0x60208000, 0x60188800, 0x60109000, 0x60089800, 0x6000a000, 0x5808a000, 0x5800a800, 0x5008a800, 0x5808a000, 0x5010a000, 0x5008a800, 0x5000b000, 0x4808b000, 0x4800b800, 0x4008b800, 0x4808b000, 0x4010b000, 0x4810a800, 0x5010a000, 0x4818a000, 0x4810a800, 0x4018a800, 0x4818a000, 0x4020a000, 0x48209800, 0x50209000, 0x58208800, 0x60208000, 0x58288000, 0x58208800, 0x50288800, 0x58288000, 0x50308000, 0x50288800, 0x50209000, 0x48289000, 0x48209800, 0x40289800, 0x48289000, 0x40309000, 0x48308800, 0x50308000, 0x48388000, 0x48308800, 0x40388800, 0x48388000, 0x40408000, 0x40388800, 0x40309000, 0x40289800, 0x4020a000, 0x4018a800, 0x4010b000, 0x4008b800, 0x4000c000, 0x3808c000, 0x3800c800, 0x3008c800, 0x3808c000, 0x3010c000, 0x3008c800, 0x3000d000, 0x2808d000, 0x2800d800, 0x2008d800, 0x2808d000, 0x2010d000, 0x2810c800, 0x3010c000, 0x2818c000, 0x2810c800, 0x2018c800, 0x2818c000, 0x2020c000, 0x2018c800, 0x2010d000, 0x2008d800, 0x2000e000, 0x1808e000, 0x1800e800, 0x1008e800, 0x1808e000, 0x1010e000, 0x1008e800, 0x1000f000, 0x808f000, 0x800f800, 0x08f800, 0x808f000, 0x10f000, 0x810e800, 0x1010e000, 0x818e000, 0x810e800, 0x18e800, 0x818e000, 0x20e000, 0x820d800, 0x1020d000, 0x1820c800, 0x2020c000, 0x1828c000, 0x1820c800, 0x1028c800, 0x1828c000, 0x1030c000, 0x1028c800, 0x1020d000, 0x828d000, 0x820d800, 0x28d800, 0x828d000, 0x30d000, 0x830c800, 0x1030c000, 0x838c000, 0x830c800, 0x38c800, 0x838c000, 0x40c000, 0x840b800, 0x1040b000, 0x1840a800, 0x2040a000, 0x28409800, 0x30409000, 0x38408800, 0x40408000, 0x38488000, 0x38408800, 0x30488800, 0x38488000, 0x30508000, 0x30488800, 0x30409000, 0x28489000, 0x28409800, 0x20489800, 0x28489000, 0x20509000, 0x28508800, 0x30508000, 0x28588000, 0x28508800, 0x20588800, 0x28588000, 0x20608000, 0x20588800, 0x20509000, 0x20489800, 0x2040a000, 0x1848a000, 0x1840a800, 0x1048a800, 0x1848a000, 0x1050a000, 0x1048a800, 0x1040b000, 0x848b000, 0x840b800, 0x48b800, 0x848b000, 0x50b000, 0x850a800, 0x1050a000, 0x858a000, 0x850a800, 0x58a800, 0x858a000, 0x60a000, 0x8609800, 0x10609000, 0x18608800, 0x20608000, 0x18688000, 0x18608800, 0x10688800, 0x18688000, 0x10708000, 0x10688800, 0x10609000, 0x8689000, 0x8609800, 0x689800, 0x8689000, 0x709000, 0x8708800, 0x10708000, 0x8788000, 0x8708800, 0x788800, 0x8788000, 0x808000, 0x8807800, 0x10807000, 0x18806800, 0x20806000, 0x28805800, 0x30805000, 0x38804800, 0x40804000, 0x48803800, 0x50803000, 0x58802800, 0x60802000, 0x68801800, 0x70801000, 0x78800800, 0x80800000, 0x78880000, 0x78800800, 0x70880800, 0x78880000, 0x70900000, 0x70880800, 0x70801000, 0x68881000, 0x68801800, 0x60881800, 0x68881000, 0x60901000, 0x68900800, 0x70900000, 0x68980000, 0x68900800, 0x60980800, 0x68980000, 0x60a00000, 0x60980800, 0x60901000, 0x60881800, 0x60802000, 0x58882000, 0x58802800, 0x50882800, 0x58882000, 0x50902000, 0x50882800, 0x50803000, 0x48883000, 0x48803800, 0x40883800, 0x48883000, 0x40903000, 0x48902800, 0x50902000, 0x48982000, 0x48902800, 0x40982800, 0x48982000, 0x40a02000, 0x48a01800, 0x50a01000, 0x58a00800, 0x60a00000, 0x58a80000, 0x58a00800, 0x50a80800, 0x58a80000, 0x50b00000, 0x50a80800, 0x50a01000, 0x48a81000, 0x48a01800, 0x40a81800, 0x48a81000, 0x40b01000, 0x48b00800, 0x50b00000, 0x48b80000, 0x48b00800, 0x40b80800, 0x48b80000, 0x40c00000, 0x40b80800, 0x40b01000, 0x40a81800, 0x40a02000, 0x40982800, 0x40903000, 0x40883800, 0x40804000, 0x38884000, 0x38804800, 0x30884800, 0x38884000, 0x30904000, 0x30884800, 0x30805000, 0x28885000, 0x28805800, 0x20885800, 0x28885000, 0x20905000, 0x28904800, 0x30904000, 0x28984000, 0x28904800, 0x20984800, 0x28984000, 0x20a04000, 0x20984800, 0x20905000, 0x20885800, 0x20806000, 0x18886000, 0x18806800, 0x10886800, 0x18886000, 0x10906000, 0x10886800, 0x10807000, 0x8887000, 0x8807800, 0x887800, 0x8887000, 0x907000, 0x8906800, 0x10906000, 0x8986000, 0x8906800, 0x986800, 0x8986000, 0xa06000, 0x8a05800, 0x10a05000, 0x18a04800, 0x20a04000, 0x18a84000, 0x18a04800, 0x10a84800, 0x18a84000, 0x10b04000, 0x10a84800, 0x10a05000, 0x8a85000, 0x8a05800, 0xa85800, 0x8a85000, 0xb05000, 0x8b04800, 0x10b04000, 0x8b84000, 0x8b04800, 0xb84800, 0x8b84000, 0xc04000, 0x8c03800, 0x10c03000, 0x18c02800, 0x20c02000, 0x28c01800, 0x30c01000, 0x38c00800, 0x40c00000, 0x38c80000, 0x38c00800, 0x30c80800, 0x38c80000, 0x30d00000, 0x30c80800, 0x30c01000, 0x28c81000, 0x28c01800, 0x20c81800, 0x28c81000, 0x20d01000, 0x28d00800, 0x30d00000, 0x28d80000, 0x28d00800, 0x20d80800, 0x28d80000, 0x20e00000, 0x20d80800, 0x20d01000, 0x20c81800, 0x20c02000, 0x18c82000, 0x18c02800, 0x10c82800, 0x18c82000, 0x10d02000, 0x10c82800, 0x10c03000, 0x8c83000, 0x8c03800, 0xc83800, 0x8c83000, 0xd03000, 0x8d02800, 0x10d02000, 0x8d82000, 0x8d02800, 0xd82800, 0x8d82000, 0xe02000, 0x8e01800, 0x10e01000, 0x18e00800, 0x20e00000, 0x18e80000, 0x18e00800, 0x10e80800, 0x18e80000, 0x10f00000, 0x10e80800, 0x10e01000, 0x8e81000, 0x8e01800, 0xe81800, 0x8e81000, 0xf01000, 0x8f00800, 0x10f00000, 0x8f80000, 0x8f00800, 0xf80800, 0x8f80000, 0xff0000, 0xf80800, 0xf01000, 0xe81800, 0xe02000, 0xd82800, 0xd03000, 0xc83800, 0xc04000, 0xb84800, 0xb05000, 0xa85800, 0xa06000, 0x986800, 0x907000, 0x887800, 0x808000, 0x788800, 0x709000, 0x689800, 0x60a000, 0x58a800, 0x50b000, 0x48b800, 0x40c000, 0x38c800, 0x30d000, 0x28d800, 0x20e000, 0x18e800, 0x10f000, 0x08f800, 0x00ff00, 0x800f800, 0x1000f000, 0x1800e800, 0x2000e000, 0x2800d800, 0x3000d000, 0x3800c800, 0x4000c000, 0x4800b800, 0x5000b000, 0x5800a800, 0x6000a000, 0x68009800, 0x70009000, 0x78008800, 0x80008000, 0x88007800, 0x90007000, 0x98006800, 0xa0006000, 0xa8005800, 0xb0005000, 0xb8004800, 0xc0004000, 0xc8003800, 0xd0003000, 0xd8002800, 0xe0002000, 0xe8001800, 0xf0001000, 0xf8000800};
volatile uint32_t fractal_path_6[2187]={0xff000000, 0xfc040000, 0xfc000400, 0xf8040400, 0xfc040000, 0xf8080000, 0xf8040400, 0xf8000800, 0xf4040800, 0xf4000c00, 0xf0040c00, 0xf4040800, 0xf0080800, 0xf4080400, 0xf8080000, 0xf40c0000, 0xf4080400, 0xf00c0400, 0xf40c0000, 0xf0100000, 0xf00c0400, 0xf0080800, 0xf0040c00, 0xf0001000, 0xec041000, 0xec001400, 0xe8041400, 0xec041000, 0xe8081000, 0xe8041400, 0xe8001800, 0xe4041800, 0xe4001c00, 0xe0041c00, 0xe4041800, 0xe0081800, 0xe4081400, 0xe8081000, 0xe40c1000, 0xe4081400, 0xe00c1400, 0xe40c1000, 0xe0101000, 0xe4100c00, 0xe8100800, 0xec100400, 0xf0100000, 0xec140000, 0xec100400, 0xe8140400, 0xec140000, 0xe8180000, 0xe8140400, 0xe8100800, 0xe4140800, 0xe4100c00, 0xe0140c00, 0xe4140800, 0xe0180800, 0xe4180400, 0xe8180000, 0xe41c0000, 0xe4180400, 0xe01c0400, 0xe41c0000, 0xe0200000, 0xe01c0400, 0xe0180800, 0xe0140c00, 0xe0101000, 0xe00c1400, 0xe0081800, 0xe0041c00, 0xe0002000, 0xdc042000, 0xdc002400, 0xd8042400, 0xdc042000, 0xd8082000, 0xd8042400, 0xd8002800, 0xd4042800, 0xd4002c00, 0xd0042c00, 0xd4042800, 0xd0082800, 0xd4082400, 0xd8082000, 0xd40c2000, 0xd4082400, 0xd00c2400, 0xd40c2000, 0xd0102000, 0xd00c2400, 0xd0082800, 0xd0042c00, 0xd0003000, 0xcc043000, 0xcc003400, 0xc8043400, 0xcc043000, 0xc8083000, 0xc8043400, 0xc8003800, 0xc4043800, 0xc4003c00, 0xc0043c00, 0xc4043800, 0xc0083800, 0xc4083400, 0xc8083000, 0xc40c3000, 0xc4083400, 0xc00c3400, 0xc40c3000, 0xc0103000, 0xc4102c00, 0xc8102800, 0xcc102400, 0xd0102000, 0xcc142000, 0xcc102400, 0xc8142400, 0xcc142000, 0xc8182000, 0xc8142400, 0xc8102800, 0xc4142800, 0xc4102c00, 0xc0142c00, 0xc4142800, 0xc0182800, 0xc4182400, 0xc8182000, 0xc41c2000, 0xc4182400, 0xc01c2400, 0xc41c2000, 0xc0202000, 0xc4201c00, 0xc8201800, 0xcc201400, 0xd0201000, 0xd4200c00, 0xd8200800, 0xdc200400, 0xe0200000, 0xdc240000, 0xdc200400, 0xd8240400, 0xdc240000, 0xd8280000, 0xd8240400, 0xd8200800, 0xd4240800, 0xd4200c00, 0xd0240c00, 0xd4240800, 0xd0280800, 0xd4280400, 0xd8280000, 0xd42c0000, 0xd4280400, 0xd02c0400, 0xd42c0000, 0xd0300000, 0xd02c0400, 0xd0280800, 0xd0240c00, 0xd0201000, 0xcc241000, 0xcc201400, 0xc8241400, 0xcc241000, 0xc8281000, 0xc8241400, 0xc8201800, 0xc4241800, 0xc4201c00, 0xc0241c00, 0xc4241800, 0xc0281800, 0xc4281400, 0xc8281000, 0xc42c1000, 0xc4281400, 0xc02c1400, 0xc42c1000, 0xc0301000, 0xc4300c00, 0xc8300800, 0xcc300400, 0xd0300000, 0xcc340000, 0xcc300400, 0xc8340400, 0xcc340000, 0xc8380000, 0xc8340400, 0xc8300800, 0xc4340800, 0xc4300c00, 0xc0340c00, 0xc4340800, 0xc0380800, 0xc4380400, 0xc8380000, 0xc43c0000, 0xc4380400, 0xc03c0400, 0xc43c0000, 0xc0400000, 0xc03c0400, 0xc0380800, 0xc0340c00, 0xc0301000, 0xc02c1400, 0xc0281800, 0xc0241c00, 0xc0202000, 0xc01c2400, 0xc0182800, 0xc0142c00, 0xc0103000, 0xc00c3400, 0xc0083800, 0xc0043c00, 0xc0004000, 0xbc044000, 0xbc004400, 0xb8044400, 0xbc044000, 0xb8084000, 0xb8044400, 0xb8004800, 0xb4044800, 0xb4004c00, 0xb0044c00, 0xb4044800, 0xb0084800, 0xb4084400, 0xb8084000, 0xb40c4000, 0xb4084400, 0xb00c4400, 0xb40c4000, 0xb0104000, 0xb00c4400, 0xb0084800, 0xb0044c00, 0xb0005000, 0xac045000, 0xac005400, 0xa8045400, 0xac045000, 0xa8085000, 0xa8045400, 0xa8005800, 0xa4045800, 0xa4005c00, 0xa0045c00, 0xa4045800, 0xa0085800, 0xa4085400, 0xa8085000, 0xa40c5000, 0xa4085400, 0xa00c5400, 0xa40c5000, 0xa0105000, 0xa4104c00, 0xa8104800, 0xac104400, 0xb0104000, 0xac144000, 0xac104400, 0xa8144400, 0xac144000, 0xa8184000, 0xa8144400, 0xa8104800, 0xa4144800, 0xa4104c00, 0xa0144c00, 0xa4144800, 0xa0184800, 0xa4184400, 0xa8184000, 0xa41c4000, 0xa4184400, 0xa01c4400, 0xa41c4000, 0xa0204000, 0xa01c4400, 0xa0184800, 0xa0144c00, 0xa0105000, 0xa00c5400, 0xa0085800, 0xa0045c00, 0xa0006000, 0x9c046000, 0x9c006400, 0x98046400, 0x9c046000, 0x98086000, 0x98046400, 0x98006800, 0x94046800, 0x94006c00, 0x90046c00, 0x94046800, 0x90086800, 0x94086400, 0x98086000, 0x940c6000, 0x94086400, 0x900c6400, 0x940c6000, 0x90106000, 0x900c6400, 0x90086800, 0x90046c00, 0x90007000, 0x8c047000, 0x8c007400, 0x88047400, 0x8c047000, 0x88087000, 0x88047400, 0x88007800, 0x84047800, 0x84007c00, 0x80047c00, 0x84047800, 0x80087800, 0x84087400, 0x88087000, 0x840c7000, 0x84087400, 0x800c7400, 0x840c7000, 0x80107000, 0x84106c00, 0x88106800, 0x8c106400, 0x90106000, 0x8c146000, 0x8c106400, 0x88146400, 0x8c146000, 0x88186000, 0x88146400, 0x88106800, 0x84146800, 0x84106c00, 0x80146c00, 0x84146800, 0x80186800, 0x84186400, 0x88186000, 0x841c6000, 0x84186400, 0x801c6400, 0x841c6000, 0x80206000, 0x84205c00, 0x88205800, 0x8c205400, 0x90205000, 0x94204c00, 0x98204800, 0x9c204400, 0xa0204000, 0x9c244000, 0x9c204400, 0x98244400, 0x9c244000, 0x98284000, 0x98244400, 0x98204800, 0x94244800, 0x94204c00, 0x90244c00, 0x94244800, 0x90284800, 0x94284400, 0x98284000, 0x942c4000, 0x94284400, 0x902c4400, 0x942c4000, 0x90304000, 0x902c4400, 0x90284800, 0x90244c00, 0x90205000, 0x8c245000, 0x8c205400, 0x88245400, 0x8c245000, 0x88285000, 0x88245400, 0x88205800, 0x84245800, 0x84205c00, 0x80245c00, 0x84245800, 0x80285800, 0x84285400, 0x88285000, 0x842c5000, 0x84285400, 0x802c5400, 0x842c5000, 0x80305000, 0x84304c00, 0x88304800, 0x8c304400, 0x90304000, 0x8c344000, 0x8c304400, 0x88344400, 0x8c344000, 0x88384000, 0x88344400, 0x88304800, 0x84344800, 0x84304c00, 0x80344c00, 0x84344800, 0x80384800, 0x84384400, 0x88384000, 0x843c4000, 0x84384400, 0x803c4400, 0x843c4000, 0x80404000, 0x84403c00, 0x88403800, 0x8c403400, 0x90403000, 0x94402c00, 0x98402800, 0x9c402400, 0xa0402000, 0xa4401c00, 0xa8401800, 0xac401400, 0xb0401000, 0xb4400c00, 0xb8400800, 0xbc400400, 0xc0400000, 0xbc440000, 0xbc400400, 0xb8440400, 0xbc440000, 0xb8480000, 0xb8440400, 0xb8400800, 0xb4440800, 0xb4400c00, 0xb0440c00, 0xb4440800, 0xb0480800, 0xb4480400, 0xb8480000, 0xb44c0000, 0xb4480400, 0xb04c0400, 0xb44c0000, 0xb0500000, 0xb04c0400, 0xb0480800, 0xb0440c00, 0xb0401000, 0xac441000, 0xac401400, 0xa8441400, 0xac441000, 0xa8481000, 0xa8441400, 0xa8401800, 0xa4441800, 0xa4401c00, 0xa0441c00, 0xa4441800, 0xa0481800, 0xa4481400, 0xa8481000, 0xa44c1000, 0xa4481400, 0xa04c1400, 0xa44c1000, 0xa0501000, 0xa4500c00, 0xa8500800, 0xac500400, 0xb0500000, 0xac540000, 0xac500400, 0xa8540400, 0xac540000, 0xa8580000, 0xa8540400, 0xa8500800, 0xa4540800, 0xa4500c00, 0xa0540c00, 0xa4540800, 0xa0580800, 0xa4580400, 0xa8580000, 0xa45c0000, 0xa4580400, 0xa05c0400, 0xa45c0000, 0xa0600000, 0xa05c0400, 0xa0580800, 0xa0540c00, 0xa0501000, 0xa04c1400, 0xa0481800, 0xa0441c00, 0xa0402000, 0x9c442000, 0x9c402400, 0x98442400, 0x9c442000, 0x98482000, 0x98442400, 0x98402800, 0x94442800, 0x94402c00, 0x90442c00, 0x94442800, 0x90482800, 0x94482400, 0x98482000, 0x944c2000, 0x94482400, 0x904c2400, 0x944c2000, 0x90502000, 0x904c2400, 0x90482800, 0x90442c00, 0x90403000, 0x8c443000, 0x8c403400, 0x88443400, 0x8c443000, 0x88483000, 0x88443400, 0x88403800, 0x84443800, 0x84403c00, 0x80443c00, 0x84443800, 0x80483800, 0x84483400, 0x88483000, 0x844c3000, 0x84483400, 0x804c3400, 0x844c3000, 0x80503000, 0x84502c00, 0x88502800, 0x8c502400, 0x90502000, 0x8c542000, 0x8c502400, 0x88542400, 0x8c542000, 0x88582000, 0x88542400, 0x88502800, 0x84542800, 0x84502c00, 0x80542c00, 0x84542800, 0x80582800, 0x84582400, 0x88582000, 0x845c2000, 0x84582400, 0x805c2400, 0x845c2000, 0x80602000, 0x84601c00, 0x88601800, 0x8c601400, 0x90601000, 0x94600c00, 0x98600800, 0x9c600400, 0xa0600000, 0x9c640000, 0x9c600400, 0x98640400, 0x9c640000, 0x98680000, 0x98640400, 0x98600800, 0x94640800, 0x94600c00, 0x90640c00, 0x94640800, 0x90680800, 0x94680400, 0x98680000, 0x946c0000, 0x94680400, 0x906c0400, 0x946c0000, 0x90700000, 0x906c0400, 0x90680800, 0x90640c00, 0x90601000, 0x8c641000, 0x8c601400, 0x88641400, 0x8c641000, 0x88681000, 0x88641400, 0x88601800, 0x84641800, 0x84601c00, 0x80641c00, 0x84641800, 0x80681800, 0x84681400, 0x88681000, 0x846c1000, 0x84681400, 0x806c1400, 0x846c1000, 0x80701000, 0x84700c00, 0x88700800, 0x8c700400, 0x90700000, 0x8c740000, 0x8c700400, 0x88740400, 0x8c740000, 0x88780000, 0x88740400, 0x88700800, 0x84740800, 0x84700c00, 0x80740c00, 0x84740800, 0x80780800, 0x84780400, 0x88780000, 0x847c0000, 0x84780400, 0x807c0400, 0x847c0000, 0x80800000, 0x807c0400, 0x80780800, 0x80740c00, 0x80701000, 0x806c1400, 0x80681800, 0x80641c00, 0x80602000, 0x805c2400, 0x80582800, 0x80542c00, 0x80503000, 0x804c3400, 0x80483800, 0x80443c00, 0x80404000, 0x803c4400, 0x80384800, 0x80344c00, 0x80305000, 0x802c5400, 0x80285800, 0x80245c00, 0x80206000, 0x801c6400, 0x80186800, 0x80146c00, 0x80107000, 0x800c7400, 0x80087800, 0x80047c00, 0x80008000, 0x7c048000, 0x7c008400, 0x78048400, 0x7c048000, 0x78088000, 0x78048400, 0x78008800, 0x74048800, 0x74008c00, 0x70048c00, 0x74048800, 0x70088800, 0x74088400, 0x78088000, 0x740c8000, 0x74088400, 0x700c8400, 0x740c8000, 0x70108000, 0x700c8400, 0x70088800, 0x70048c00, 0x70009000, 0x6c049000, 0x6c009400, 0x68049400, 0x6c049000, 0x68089000, 0x68049400, 0x68009800, 0x64049800, 0x64009c00, 0x60049c00, 0x64049800, 0x60089800, 0x64089400, 0x68089000, 0x640c9000, 0x64089400, 0x600c9400, 0x640c9000, 0x60109000, 0x64108c00, 0x68108800, 0x6c108400, 0x70108000, 0x6c148000, 0x6c108400, 0x68148400, 0x6c148000, 0x68188000, 0x68148400, 0x68108800, 0x64148800, 0x64108c00, 0x60148c00, 0x64148800, 0x60188800, 0x64188400, 0x68188000, 0x641c8000, 0x64188400, 0x601c8400, 0x641c8000, 0x60208000, 0x601c8400, 0x60188800, 0x60148c00, 0x60109000, 0x600c9400, 0x60089800, 0x60049c00, 0x6000a000, 0x5c04a000, 0x5c00a400, 0x5804a400, 0x5c04a000, 0x5808a000, 0x5804a400, 0x5800a800, 0x5404a800, 0x5400ac00, 0x5004ac00, 0x5404a800, 0x5008a800, 0x5408a400, 0x5808a000, 0x540ca000, 0x5408a400, 0x500ca400, 0x540ca000, 0x5010a000, 0x500ca400, 0x5008a800, 0x5004ac00, 0x5000b000, 0x4c04b000, 0x4c00b400, 0x4804b400, 0x4c04b000, 0x4808b000, 0x4804b400, 0x4800b800, 0x4404b800, 0x4400bc00, 0x4004bc00, 0x4404b800, 0x4008b800, 0x4408b400, 0x4808b000, 0x440cb000, 0x4408b400, 0x400cb400, 0x440cb000, 0x4010b000, 0x4410ac00, 0x4810a800, 0x4c10a400, 0x5010a000, 0x4c14a000, 0x4c10a400, 0x4814a400, 0x4c14a000, 0x4818a000, 0x4814a400, 0x4810a800, 0x4414a800, 0x4410ac00, 0x4014ac00, 0x4414a800, 0x4018a800, 0x4418a400, 0x4818a000, 0x441ca000, 0x4418a400, 0x401ca400, 0x441ca000, 0x4020a000, 0x44209c00, 0x48209800, 0x4c209400, 0x50209000, 0x54208c00, 0x58208800, 0x5c208400, 0x60208000, 0x5c248000, 0x5c208400, 0x58248400, 0x5c248000, 0x58288000, 0x58248400, 0x58208800, 0x54248800, 0x54208c00, 0x50248c00, 0x54248800, 0x50288800, 0x54288400, 0x58288000, 0x542c8000, 0x54288400, 0x502c8400, 0x542c8000, 0x50308000, 0x502c8400, 0x50288800, 0x50248c00, 0x50209000, 0x4c249000, 0x4c209400, 0x48249400, 0x4c249000, 0x48289000, 0x48249400, 0x48209800, 0x44249800, 0x44209c00, 0x40249c00, 0x44249800, 0x40289800, 0x44289400, 0x48289000, 0x442c9000, 0x44289400, 0x402c9400, 0x442c9000, 0x40309000, 0x44308c00, 0x48308800, 0x4c308400, 0x50308000, 0x4c348000, 0x4c308400, 0x48348400, 0x4c348000, 0x48388000, 0x48348400, 0x48308800, 0x44348800, 0x44308c00, 0x40348c00, 0x44348800, 0x40388800, 0x44388400, 0x48388000, 0x443c8000, 0x44388400, 0x403c8400, 0x443c8000, 0x40408000, 0x403c8400, 0x40388800, 0x40348c00, 0x40309000, 0x402c9400, 0x40289800, 0x40249c00, 0x4020a000, 0x401ca400, 0x4018a800, 0x4014ac00, 0x4010b000, 0x400cb400, 0x4008b800, 0x4004bc00, 0x4000c000, 0x3c04c000, 0x3c00c400, 0x3804c400, 0x3c04c000, 0x3808c000, 0x3804c400, 0x3800c800, 0x3404c800, 0x3400cc00, 0x3004cc00, 0x3404c800, 0x3008c800, 0x3408c400, 0x3808c000, 0x340cc000, 0x3408c400, 0x300cc400, 0x340cc000, 0x3010c000, 0x300cc400, 0x3008c800, 0x3004cc00, 0x3000d000, 0x2c04d000, 0x2c00d400, 0x2804d400, 0x2c04d000, 0x2808d000, 0x2804d400, 0x2800d800, 0x2404d800, 0x2400dc00, 0x2004dc00, 0x2404d800, 0x2008d800, 0x2408d400, 0x2808d000, 0x240cd000, 0x2408d400, 0x200cd400, 0x240cd000, 0x2010d000, 0x2410cc00, 0x2810c800, 0x2c10c400, 0x3010c000, 0x2c14c000, 0x2c10c400, 0x2814c400, 0x2c14c000, 0x2818c000, 0x2814c400, 0x2810c800, 0x2414c800, 0x2410cc00, 0x2014cc00, 0x2414c800, 0x2018c800, 0x2418c400, 0x2818c000, 0x241cc000, 0x2418c400, 0x201cc400, 0x241cc000, 0x2020c000, 0x201cc400, 0x2018c800, 0x2014cc00, 0x2010d000, 0x200cd400, 0x2008d800, 0x2004dc00, 0x2000e000, 0x1c04e000, 0x1c00e400, 0x1804e400, 0x1c04e000, 0x1808e000, 0x1804e400, 0x1800e800, 0x1404e800, 0x1400ec00, 0x1004ec00, 0x1404e800, 0x1008e800, 0x1408e400, 0x1808e000, 0x140ce000, 0x1408e400, 0x100ce400, 0x140ce000, 0x1010e000, 0x100ce400, 0x1008e800, 0x1004ec00, 0x1000f000, 0xc04f000, 0xc00f400, 0x804f400, 0xc04f000, 0x808f000, 0x804f400, 0x800f800, 0x404f800, 0x400fc00, 0x04fc00, 0x404f800, 0x08f800, 0x408f400, 0x808f000, 0x40cf000, 0x408f400, 0x0cf400, 0x40cf000, 0x10f000, 0x410ec00, 0x810e800, 0xc10e400, 0x1010e000, 0xc14e000, 0xc10e400, 0x814e400, 0xc14e000, 0x818e000, 0x814e400, 0x810e800, 0x414e800, 0x410ec00, 0x14ec00, 0x414e800, 0x18e800, 0x418e400, 0x818e000, 0x41ce000, 0x418e400, 0x1ce400, 0x41ce000, 0x20e000, 0x420dc00, 0x820d800, 0xc20d400, 0x1020d000, 0x1420cc00, 0x1820c800, 0x1c20c400, 0x2020c000, 0x1c24c000, 0x1c20c400, 0x1824c400, 0x1c24c000, 0x1828c000, 0x1824c400, 0x1820c800, 0x1424c800, 0x1420cc00, 0x1024cc00, 0x1424c800, 0x1028c800, 0x1428c400, 0x1828c000, 0x142cc000, 0x1428c400, 0x102cc400, 0x142cc000, 0x1030c000, 0x102cc400, 0x1028c800, 0x1024cc00, 0x1020d000, 0xc24d000, 0xc20d400, 0x824d400, 0xc24d000, 0x828d000, 0x824d400, 0x820d800, 0x424d800, 0x420dc00, 0x24dc00, 0x424d800, 0x28d800, 0x428d400, 0x828d000, 0x42cd000, 0x428d400, 0x2cd400, 0x42cd000, 0x30d000, 0x430cc00, 0x830c800, 0xc30c400, 0x1030c000, 0xc34c000, 0xc30c400, 0x834c400, 0xc34c000, 0x838c000, 0x834c400, 0x830c800, 0x434c800, 0x430cc00, 0x34cc00, 0x434c800, 0x38c800, 0x438c400, 0x838c000, 0x43cc000, 0x438c400, 0x3cc400, 0x43cc000, 0x40c000, 0x440bc00, 0x840b800, 0xc40b400, 0x1040b000, 0x1440ac00, 0x1840a800, 0x1c40a400, 0x2040a000, 0x24409c00, 0x28409800, 0x2c409400, 0x30409000, 0x34408c00, 0x38408800, 0x3c408400, 0x40408000, 0x3c448000, 0x3c408400, 0x38448400, 0x3c448000, 0x38488000, 0x38448400, 0x38408800, 0x34448800, 0x34408c00, 0x30448c00, 0x34448800, 0x30488800, 0x34488400, 0x38488000, 0x344c8000, 0x34488400, 0x304c8400, 0x344c8000, 0x30508000, 0x304c8400, 0x30488800, 0x30448c00, 0x30409000, 0x2c449000, 0x2c409400, 0x28449400, 0x2c449000, 0x28489000, 0x28449400, 0x28409800, 0x24449800, 0x24409c00, 0x20449c00, 0x24449800, 0x20489800, 0x24489400, 0x28489000, 0x244c9000, 0x24489400, 0x204c9400, 0x244c9000, 0x20509000, 0x24508c00, 0x28508800, 0x2c508400, 0x30508000, 0x2c548000, 0x2c508400, 0x28548400, 0x2c548000, 0x28588000, 0x28548400, 0x28508800, 0x24548800, 0x24508c00, 0x20548c00, 0x24548800, 0x20588800, 0x24588400, 0x28588000, 0x245c8000, 0x24588400, 0x205c8400, 0x245c8000, 0x20608000, 0x205c8400, 0x20588800, 0x20548c00, 0x20509000, 0x204c9400, 0x20489800, 0x20449c00, 0x2040a000, 0x1c44a000, 0x1c40a400, 0x1844a400, 0x1c44a000, 0x1848a000, 0x1844a400, 0x1840a800, 0x1444a800, 0x1440ac00, 0x1044ac00, 0x1444a800, 0x1048a800, 0x1448a400, 0x1848a000, 0x144ca000, 0x1448a400, 0x104ca400, 0x144ca000, 0x1050a000, 0x104ca400, 0x1048a800, 0x1044ac00, 0x1040b000, 0xc44b000, 0xc40b400, 0x844b400, 0xc44b000, 0x848b000, 0x844b400, 0x840b800, 0x444b800, 0x440bc00, 0x44bc00, 0x444b800, 0x48b800, 0x448b400, 0x848b000, 0x44cb000, 0x448b400, 0x4cb400, 0x44cb000, 0x50b000, 0x450ac00, 0x850a800, 0xc50a400, 0x1050a000, 0xc54a000, 0xc50a400, 0x854a400, 0xc54a000, 0x858a000, 0x854a400, 0x850a800, 0x454a800, 0x450ac00, 0x54ac00, 0x454a800, 0x58a800, 0x458a400, 0x858a000, 0x45ca000, 0x458a400, 0x5ca400, 0x45ca000, 0x60a000, 0x4609c00, 0x8609800, 0xc609400, 0x10609000, 0x14608c00, 0x18608800, 0x1c608400, 0x20608000, 0x1c648000, 0x1c608400, 0x18648400, 0x1c648000, 0x18688000, 0x18648400, 0x18608800, 0x14648800, 0x14608c00, 0x10648c00, 0x14648800, 0x10688800, 0x14688400, 0x18688000, 0x146c8000, 0x14688400, 0x106c8400, 0x146c8000, 0x10708000, 0x106c8400, 0x10688800, 0x10648c00, 0x10609000, 0xc649000, 0xc609400, 0x8649400, 0xc649000, 0x8689000, 0x8649400, 0x8609800, 0x4649800, 0x4609c00, 0x649c00, 0x4649800, 0x689800, 0x4689400, 0x8689000, 0x46c9000, 0x4689400, 0x6c9400, 0x46c9000, 0x709000, 0x4708c00, 0x8708800, 0xc708400, 0x10708000, 0xc748000, 0xc708400, 0x8748400, 0xc748000, 0x8788000, 0x8748400, 0x8708800, 0x4748800, 0x4708c00, 0x748c00, 0x4748800, 0x788800, 0x4788400, 0x8788000, 0x47c8000, 0x4788400, 0x7c8400, 0x47c8000, 0x808000, 0x4807c00, 0x8807800, 0xc807400, 0x10807000, 0x14806c00, 0x18806800, 0x1c806400, 0x20806000, 0x24805c00, 0x28805800, 0x2c805400, 0x30805000, 0x34804c00, 0x38804800, 0x3c804400, 0x40804000, 0x44803c00, 0x48803800, 0x4c803400, 0x50803000, 0x54802c00, 0x58802800, 0x5c802400, 0x60802000, 0x64801c00, 0x68801800, 0x6c801400, 0x70801000, 0x74800c00, 0x78800800, 0x7c800400, 0x80800000, 0x7c840000, 0x7c800400, 0x78840400, 0x7c840000, 0x78880000, 0x78840400, 0x78800800, 0x74840800, 0x74800c00, 0x70840c00, 0x74840800, 0x70880800, 0x74880400, 0x78880000, 0x748c0000, 0x74880400, 0x708c0400, 0x748c0000, 0x70900000, 0x708c0400, 0x70880800, 0x70840c00, 0x70801000, 0x6c841000, 0x6c801400, 0x68841400, 0x6c841000, 0x68881000, 0x68841400, 0x68801800, 0x64841800, 0x64801c00, 0x60841c00, 0x64841800, 0x60881800, 0x64881400, 0x68881000, 0x648c1000, 0x64881400, 0x608c1400, 0x648c1000, 0x60901000, 0x64900c00, 0x68900800, 0x6c900400, 0x70900000, 0x6c940000, 0x6c900400, 0x68940400, 0x6c940000, 0x68980000, 0x68940400, 0x68900800, 0x64940800, 0x64900c00, 0x60940c00, 0x64940800, 0x60980800, 0x64980400, 0x68980000, 0x649c0000, 0x64980400, 0x609c0400, 0x649c0000, 0x60a00000, 0x609c0400, 0x60980800, 0x60940c00, 0x60901000, 0x608c1400, 0x60881800, 0x60841c00, 0x60802000, 0x5c842000, 0x5c802400, 0x58842400, 0x5c842000, 0x58882000, 0x58842400, 0x58802800, 0x54842800, 0x54802c00, 0x50842c00, 0x54842800, 0x50882800, 0x54882400, 0x58882000, 0x548c2000, 0x54882400, 0x508c2400, 0x548c2000, 0x50902000, 0x508c2400, 0x50882800, 0x50842c00, 0x50803000, 0x4c843000, 0x4c803400, 0x48843400, 0x4c843000, 0x48883000, 0x48843400, 0x48803800, 0x44843800, 0x44803c00, 0x40843c00, 0x44843800, 0x40883800, 0x44883400, 0x48883000, 0x448c3000, 0x44883400, 0x408c3400, 0x448c3000, 0x40903000, 0x44902c00, 0x48902800, 0x4c902400, 0x50902000, 0x4c942000, 0x4c902400, 0x48942400, 0x4c942000, 0x48982000, 0x48942400, 0x48902800, 0x44942800, 0x44902c00, 0x40942c00, 0x44942800, 0x40982800, 0x44982400, 0x48982000, 0x449c2000, 0x44982400, 0x409c2400, 0x449c2000, 0x40a02000, 0x44a01c00, 0x48a01800, 0x4ca01400, 0x50a01000, 0x54a00c00, 0x58a00800, 0x5ca00400, 0x60a00000, 0x5ca40000, 0x5ca00400, 0x58a40400, 0x5ca40000, 0x58a80000, 0x58a40400, 0x58a00800, 0x54a40800, 0x54a00c00, 0x50a40c00, 0x54a40800, 0x50a80800, 0x54a80400, 0x58a80000, 0x54ac0000, 0x54a80400, 0x50ac0400, 0x54ac0000, 0x50b00000, 0x50ac0400, 0x50a80800, 0x50a40c00, 0x50a01000, 0x4ca41000, 0x4ca01400, 0x48a41400, 0x4ca41000, 0x48a81000, 0x48a41400, 0x48a01800, 0x44a41800, 0x44a01c00, 0x40a41c00, 0x44a41800, 0x40a81800, 0x44a81400, 0x48a81000, 0x44ac1000, 0x44a81400, 0x40ac1400, 0x44ac1000, 0x40b01000, 0x44b00c00, 0x48b00800, 0x4cb00400, 0x50b00000, 0x4cb40000, 0x4cb00400, 0x48b40400, 0x4cb40000, 0x48b80000, 0x48b40400, 0x48b00800, 0x44b40800, 0x44b00c00, 0x40b40c00, 0x44b40800, 0x40b80800, 0x44b80400, 0x48b80000, 0x44bc0000, 0x44b80400, 0x40bc0400, 0x44bc0000, 0x40c00000, 0x40bc0400, 0x40b80800, 0x40b40c00, 0x40b01000, 0x40ac1400, 0x40a81800, 0x40a41c00, 0x40a02000, 0x409c2400, 0x40982800, 0x40942c00, 0x40903000, 0x408c3400, 0x40883800, 0x40843c00, 0x40804000, 0x3c844000, 0x3c804400, 0x38844400, 0x3c844000, 0x38884000, 0x38844400, 0x38804800, 0x34844800, 0x34804c00, 0x30844c00, 0x34844800, 0x30884800, 0x34884400, 0x38884000, 0x348c4000, 0x34884400, 0x308c4400, 0x348c4000, 0x30904000, 0x308c4400, 0x30884800, 0x30844c00, 0x30805000, 0x2c845000, 0x2c805400, 0x28845400, 0x2c845000, 0x28885000, 0x28845400, 0x28805800, 0x24845800, 0x24805c00, 0x20845c00, 0x24845800, 0x20885800, 0x24885400, 0x28885000, 0x248c5000, 0x24885400, 0x208c5400, 0x248c5000, 0x20905000, 0x24904c00, 0x28904800, 0x2c904400, 0x30904000, 0x2c944000, 0x2c904400, 0x28944400, 0x2c944000, 0x28984000, 0x28944400, 0x28904800, 0x24944800, 0x24904c00, 0x20944c00, 0x24944800, 0x20984800, 0x24984400, 0x28984000, 0x249c4000, 0x24984400, 0x209c4400, 0x249c4000, 0x20a04000, 0x209c4400, 0x20984800, 0x20944c00, 0x20905000, 0x208c5400, 0x20885800, 0x20845c00, 0x20806000, 0x1c846000, 0x1c806400, 0x18846400, 0x1c846000, 0x18886000, 0x18846400, 0x18806800, 0x14846800, 0x14806c00, 0x10846c00, 0x14846800, 0x10886800, 0x14886400, 0x18886000, 0x148c6000, 0x14886400, 0x108c6400, 0x148c6000, 0x10906000, 0x108c6400, 0x10886800, 0x10846c00, 0x10807000, 0xc847000, 0xc807400, 0x8847400, 0xc847000, 0x8887000, 0x8847400, 0x8807800, 0x4847800, 0x4807c00, 0x847c00, 0x4847800, 0x887800, 0x4887400, 0x8887000, 0x48c7000, 0x4887400, 0x8c7400, 0x48c7000, 0x907000, 0x4906c00, 0x8906800, 0xc906400, 0x10906000, 0xc946000, 0xc906400, 0x8946400, 0xc946000, 0x8986000, 0x8946400, 0x8906800, 0x4946800, 0x4906c00, 0x946c00, 0x4946800, 0x986800, 0x4986400, 0x8986000, 0x49c6000, 0x4986400, 0x9c6400, 0x49c6000, 0xa06000, 0x4a05c00, 0x8a05800, 0xca05400, 0x10a05000, 0x14a04c00, 0x18a04800, 0x1ca04400, 0x20a04000, 0x1ca44000, 0x1ca04400, 0x18a44400, 0x1ca44000, 0x18a84000, 0x18a44400, 0x18a04800, 0x14a44800, 0x14a04c00, 0x10a44c00, 0x14a44800, 0x10a84800, 0x14a84400, 0x18a84000, 0x14ac4000, 0x14a84400, 0x10ac4400, 0x14ac4000, 0x10b04000, 0x10ac4400, 0x10a84800, 0x10a44c00, 0x10a05000, 0xca45000, 0xca05400, 0x8a45400, 0xca45000, 0x8a85000, 0x8a45400, 0x8a05800, 0x4a45800, 0x4a05c00, 0xa45c00, 0x4a45800, 0xa85800, 0x4a85400, 0x8a85000, 0x4ac5000, 0x4a85400, 0xac5400, 0x4ac5000, 0xb05000, 0x4b04c00, 0x8b04800, 0xcb04400, 0x10b04000, 0xcb44000, 0xcb04400, 0x8b44400, 0xcb44000, 0x8b84000, 0x8b44400, 0x8b04800, 0x4b44800, 0x4b04c00, 0xb44c00, 0x4b44800, 0xb84800, 0x4b84400, 0x8b84000, 0x4bc4000, 0x4b84400, 0xbc4400, 0x4bc4000, 0xc04000, 0x4c03c00, 0x8c03800, 0xcc03400, 0x10c03000, 0x14c02c00, 0x18c02800, 0x1cc02400, 0x20c02000, 0x24c01c00, 0x28c01800, 0x2cc01400, 0x30c01000, 0x34c00c00, 0x38c00800, 0x3cc00400, 0x40c00000, 0x3cc40000, 0x3cc00400, 0x38c40400, 0x3cc40000, 0x38c80000, 0x38c40400, 0x38c00800, 0x34c40800, 0x34c00c00, 0x30c40c00, 0x34c40800, 0x30c80800, 0x34c80400, 0x38c80000, 0x34cc0000, 0x34c80400, 0x30cc0400, 0x34cc0000, 0x30d00000, 0x30cc0400, 0x30c80800, 0x30c40c00, 0x30c01000, 0x2cc41000, 0x2cc01400, 0x28c41400, 0x2cc41000, 0x28c81000, 0x28c41400, 0x28c01800, 0x24c41800, 0x24c01c00, 0x20c41c00, 0x24c41800, 0x20c81800, 0x24c81400, 0x28c81000, 0x24cc1000, 0x24c81400, 0x20cc1400, 0x24cc1000, 0x20d01000, 0x24d00c00, 0x28d00800, 0x2cd00400, 0x30d00000, 0x2cd40000, 0x2cd00400, 0x28d40400, 0x2cd40000, 0x28d80000, 0x28d40400, 0x28d00800, 0x24d40800, 0x24d00c00, 0x20d40c00, 0x24d40800, 0x20d80800, 0x24d80400, 0x28d80000, 0x24dc0000, 0x24d80400, 0x20dc0400, 0x24dc0000, 0x20e00000, 0x20dc0400, 0x20d80800, 0x20d40c00, 0x20d01000, 0x20cc1400, 0x20c81800, 0x20c41c00, 0x20c02000, 0x1cc42000, 0x1cc02400, 0x18c42400, 0x1cc42000, 0x18c82000, 0x18c42400, 0x18c02800, 0x14c42800, 0x14c02c00, 0x10c42c00, 0x14c42800, 0x10c82800, 0x14c82400, 0x18c82000, 0x14cc2000, 0x14c82400, 0x10cc2400, 0x14cc2000, 0x10d02000, 0x10cc2400, 0x10c82800, 0x10c42c00, 0x10c03000, 0xcc43000, 0xcc03400, 0x8c43400, 0xcc43000, 0x8c83000, 0x8c43400, 0x8c03800, 0x4c43800, 0x4c03c00, 0xc43c00, 0x4c43800, 0xc83800, 0x4c83400, 0x8c83000, 0x4cc3000, 0x4c83400, 0xcc3400, 0x4cc3000, 0xd03000, 0x4d02c00, 0x8d02800, 0xcd02400, 0x10d02000, 0xcd42000, 0xcd02400, 0x8d42400, 0xcd42000, 0x8d82000, 0x8d42400, 0x8d02800, 0x4d42800, 0x4d02c00, 0xd42c00, 0x4d42800, 0xd82800, 0x4d82400, 0x8d82000, 0x4dc2000, 0x4d82400, 0xdc2400, 0x4dc2000, 0xe02000, 0x4e01c00, 0x8e01800, 0xce01400, 0x10e01000, 0x14e00c00, 0x18e00800, 0x1ce00400, 0x20e00000, 0x1ce40000, 0x1ce00400, 0x18e40400, 0x1ce40000, 0x18e80000, 0x18e40400, 0x18e00800, 0x14e40800, 0x14e00c00, 0x10e40c00, 0x14e40800, 0x10e80800, 0x14e80400, 0x18e80000, 0x14ec0000, 0x14e80400, 0x10ec0400, 0x14ec0000, 0x10f00000, 0x10ec0400, 0x10e80800, 0x10e40c00, 0x10e01000, 0xce41000, 0xce01400, 0x8e41400, 0xce41000, 0x8e81000, 0x8e41400, 0x8e01800, 0x4e41800, 0x4e01c00, 0xe41c00, 0x4e41800, 0xe81800, 0x4e81400, 0x8e81000, 0x4ec1000, 0x4e81400, 0xec1400, 0x4ec1000, 0xf01000, 0x4f00c00, 0x8f00800, 0xcf00400, 0x10f00000, 0xcf40000, 0xcf00400, 0x8f40400, 0xcf40000, 0x8f80000, 0x8f40400, 0x8f00800, 0x4f40800, 0x4f00c00, 0xf40c00, 0x4f40800, 0xf80800, 0x4f80400, 0x8f80000, 0x4fc0000, 0x4f80400, 0xfc0400, 0x4fc0000, 0xff0000, 0xfc0400, 0xf80800, 0xf40c00, 0xf01000, 0xec1400, 0xe81800, 0xe41c00, 0xe02000, 0xdc2400, 0xd82800, 0xd42c00, 0xd03000, 0xcc3400, 0xc83800, 0xc43c00, 0xc04000, 0xbc4400, 0xb84800, 0xb44c00, 0xb05000, 0xac5400, 0xa85800, 0xa45c00, 0xa06000, 0x9c6400, 0x986800, 0x946c00, 0x907000, 0x8c7400, 0x887800, 0x847c00, 0x808000, 0x7c8400, 0x788800, 0x748c00, 0x709000, 0x6c9400, 0x689800, 0x649c00, 0x60a000, 0x5ca400, 0x58a800, 0x54ac00, 0x50b000, 0x4cb400, 0x48b800, 0x44bc00, 0x40c000, 0x3cc400, 0x38c800, 0x34cc00, 0x30d000, 0x2cd400, 0x28d800, 0x24dc00, 0x20e000, 0x1ce400, 0x18e800, 0x14ec00, 0x10f000, 0x0cf400, 0x08f800, 0x04fc00, 0x00ff00, 0x400fc00, 0x800f800, 0xc00f400, 0x1000f000, 0x1400ec00, 0x1800e800, 0x1c00e400, 0x2000e000, 0x2400dc00, 0x2800d800, 0x2c00d400, 0x3000d000, 0x3400cc00, 0x3800c800, 0x3c00c400, 0x4000c000, 0x4400bc00, 0x4800b800, 0x4c00b400, 0x5000b000, 0x5400ac00, 0x5800a800, 0x5c00a400, 0x6000a000, 0x64009c00, 0x68009800, 0x6c009400, 0x70009000, 0x74008c00, 0x78008800, 0x7c008400, 0x80008000, 0x84007c00, 0x88007800, 0x8c007400, 0x90007000, 0x94006c00, 0x98006800, 0x9c006400, 0xa0006000, 0xa4005c00, 0xa8005800, 0xac005400, 0xb0005000, 0xb4004c00, 0xb8004800, 0xbc004400, 0xc0004000, 0xc4003c00, 0xc8003800, 0xcc003400, 0xd0003000, 0xd4002c00, 0xd8002800, 0xdc002400, 0xe0002000, 0xe4001c00, 0xe8001800, 0xec001400, 0xf0001000, 0xf4000c00, 0xf8000800, 0xfc000400};

volatile uint32_t *fractal_path_ptrs[7]={&fractal_path_0[0],&fractal_path_1[0],&fractal_path_2[0],&fractal_path_3[0],&fractal_path_4[0],&fractal_path_5[0],&fractal_path_6[0]};
uint16_t fractal_path_lengths[7]={3,9,27,81,243,729,2187};

uint16_t test_mat[3][3]={{1,2,3},{4,5,6},{7,8,9}};
#endif