/*
 * s_curve.h
 *
 *  Created on: 2022. 7. 31.
 *      Author: gns2l
 */

#ifndef SRC_COMMON_CORE_S_CURVE_H_
#define SRC_COMMON_CORE_S_CURVE_H_



#define TABLE_LEN   200

//================================================
const unsigned int u32_Speed0[]={
2100000,//0
1600000,
1400000,
1200000,
900000,
700000,
600000,
500000,
400000,
300000,
160000,//10
140000,
130000,
120000,
100000,
790000,
750000,
720000,
69000,
66000,
63000,//20
61000,
58000,
56000,
54000,
52000,
52000,
51000,
49000,
47000,
45000,//30
43000,
41000,
39000,
35000,
33000,
32000,
32000,
31000,
30000,
29000,//40
28000,
27000,
26000,
25000,
24000,
23000,
22000,
21000,
20000,
19000,//50
18000,
17000,
16000,
15000,
14000,
13000,
12000,
11000,
10000,
};

//================================================




//================================================
const unsigned int u32_Speed[]={
2100000,//0
1600000,
1400000,
1200000,
900000,
700000,
600000,
500000,
400000,
300000,
160000,//10
150000,
140000,
130000,
120000,
110000,
100000,
99000,
98000,
97000,
96000,//20
95000,
94000,
93000,
92000,
91000,
90000,
89000,
88000,
87000,
86000,//30
85000,
84000,
83000,
82000,
81000,
80000,
79000,
78000,
77000,
76000,//40
75000,
74000,
73000,
72000,
71000,
70000,
69000,
68000,
67000,
66000,//50
65000,
64000,
63000,
62000,
61000,
60000,
59000,
58000,
57000,
56000,//60
55000,
54000,
53000,
52000,
51000,
50000,
49000,
48000,
47000,
46000,//70
45000,
44000,
43000,
42000,
41000,
40000,
39000,
38000,
37000,
36000,//80
35000,
34000,
33000,
32000,
31000,
32000,
31000,
30000,
29000,
28000,//90
27000,
26000,
25000,
24000,
23000,
22000,
21000,
20000,
19000,
18000,//100
17000,
16000,
15000,
14000,
13000,
12000,
11000,
10000,
9000,
8000,//110
7000,
6000,
5000,
4000,
3000,
2000,
1000,//116
};

//================================================


//================================================

unsigned int s_curve_table[TABLE_LEN]=
{
133,
134,
136,
138,
140,
142,
144,
146,
148,
150,
153,
155,
158,
160,
163,
166,
169,
173,
176,
179,
183,
187,
191,
195,
199,
203,
208,
212,
217,
222,
227,
233,
238,
244,
249,
255,
262,
268,
274,
281,
288,
295,
302,
309,
317,
324,
332,
340,
348,
356,
364,
372,
380,
389,
397,
406,
415,
423,
432,
441,
450,
458,
467,
476,
484,
493,
502,
510,
519,
527,
535,
543,
551,
559,
567,
575,
582,
590,
597,
604,
611,
618,
625,
631,
637,
644,
650,
655,
661,
666,
672,
677,
682,
687,
691,
696,
700,
704,
708,
712,
716,
720,
723,
726,
730,
733,
736,
739,
741,
744,
746,
749,
751,
753,
755,
757,
759,
761,
763,
765,
766,
768,
769,
771,
772,
773,
775,
776,
777,
778,
779,
780,
781,
782,
783,
783,
784,
785,
786,
786,
787,
788,
788,
789,
789,
790,
790,
791,
791,
791,
792,
792,
793,
793,
793,
793,
794,
794,
794,
795,
795,
795,
795,
795,
796,
796,
796,
796,
796,
797,
797,
797,
797,
797,
797,
797,
797,
797,
798,
798,
798,
798,
798,
798,
798,
798,
798,
798,
798,
798,
798,
799,
799,
799,
799,
799,
799,
799,
799,
799,
};

//================================================





#endif /* SRC_COMMON_CORE_S_CURVE_H_ */
