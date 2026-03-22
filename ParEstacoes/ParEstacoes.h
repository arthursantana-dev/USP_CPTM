#ifndef PARESTACOES_H
    #define PARESTACOES_H

    typedef struct
    {
        int origem;
        int destino;
    } ParEstacoes;

    int comparar_pares(const void *a, const void *b)
	{
		ParEstacoes *p1 = (ParEstacoes *)a;
		ParEstacoes *p2 = (ParEstacoes *)b;

		if(p1->origem != p2->origem) {
			return p1->origem - p2->origem;
		}

		return p1->destino - p2->destino;
	}
    
#endif


