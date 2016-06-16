GLuint LoadTexture( int neco, const char * filename, int width, int height, int pruhlednost )
{
    GLuint texture;
    unsigned char * data;
    FILE * obrazek;
    
    obrazek = fopen( filename, "rb" );
    if ( obrazek == NULL ) return 0;
    data = (unsigned char *)malloc( ((width) * height * 4));
    for(int poradi902=0;poradi902<54;poradi902++){fgetc( obrazek);}
    int red900,green900,blue900;
	int mezera2009 = width % 4;
    for(int poradi901=0;poradi901<height;poradi901++)
    {
       for(int poradi900=0;poradi900<width;poradi900++)
       {
       	  red900 = fgetc( obrazek);
       	  green900 = fgetc( obrazek);
       	  blue900 = fgetc( obrazek);
   	      *(data +(4*(width)*poradi901) +(4*poradi900) +0) = red900;
   	      *(data +(4*(width)*poradi901) +(4*poradi900) +1) = green900;
   	      *(data +(4*(width)*poradi901) +(4*poradi900) +2) = blue900;
   	      if((blue900==0)&(green900==255)&(red900==255))
          {
             *(data +(4*(width)*poradi901) +(4*poradi900) +3) = 0;
		  }else
		  {
   	         *(data +(4*(width)*poradi901) +(4*poradi900) +3) = pruhlednost;
   	      }
   	      
	   }
	   for(int poradi2009=0;poradi2009<mezera2009;poradi2009++){fgetc( obrazek);}
	}
    fclose( obrazek );

    glGenTextures( neco, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    free( data );
    return texture;
}


int LoadClickMask( int neco, const char * filename, int width, int height, int pruhlednost, bool* clickmask1)
{
    GLuint texture;
    unsigned char * data;
    FILE * obrazek;
    
    obrazek = fopen( filename, "rb" );
    if ( obrazek == NULL ) return 0;
    data = (unsigned char *)malloc( ((width) * height * 4));
    for(int poradi902=0;poradi902<54;poradi902++){fgetc( obrazek);}
    int red900,green900,blue900;
	int mezera2009 = width % 4;
    for(int poradi901=0;poradi901<height;poradi901++)
    {
       for(int poradi900=0;poradi900<width;poradi900++)
       {
       	  red900 = fgetc( obrazek);
       	  green900 = fgetc( obrazek);
       	  blue900 = fgetc( obrazek);
   	      *(data +(4*(width)*poradi901) +(4*poradi900) +0) = red900;
   	      *(data +(4*(width)*poradi901) +(4*poradi900) +1) = green900;
   	      *(data +(4*(width)*poradi901) +(4*poradi900) +2) = blue900;
   	      if((blue900==0)&(green900==255)&(red900==255))
          {
             *(clickmask1+poradi900+poradi901*width)=0;
		  }else
		  {
   	         *(clickmask1+poradi900+poradi901*width)=1;
   	      }
   	      
	   }
	   for(int poradi2009=0;poradi2009<mezera2009;poradi2009++){fgetc( obrazek);}
	}
    fclose( obrazek );

    glGenTextures( neco, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
    free( data );
    return texture;
}



void FreeTexture( GLuint texture )
{
  glDeleteTextures( 1, &texture );
}
