set(docbook_articles_EN
  TEMPLATE.xml
  about.xml
  build_pattern.xml
  build_region.xml
  ebm_primitive.xml
  mgedrc.xml
  nirt.xml
  oed.xml
  pipes.xml
  projection_shader.xml
  tire.xml
  )

set(docbook_articles_EN_IMAGES
  images/build_pattern_fig01.png
  images/build_pattern_fig02.png
  images/build_pattern_fig03.png
  images/build_pattern_fig04.png
  images/build_pattern_fig05.png
  images/build_pattern_fig06.png
  images/build_pattern_fig07.png
  images/build_region_fig01.png
  images/build_region_fig02.png
  images/build_region_fig03.png
  images/build_region_fig04.png
  images/ebm_primitive_fig01.png
  images/ebm_primitive_fig02.png
  images/ebm_primitive_fig03.png
  images/mgedrc_fig01.png
  images/mgedrc_fig02.png
  images/mgedrc_fig03.png
  images/nirt_fig01.png
  images/nirt_fig02.png
  images/nirt_fig03.png
  images/nirt_fig04.png
  images/nirt_fig05.png
  images/nirt_fig06.png
  images/nirt_fig07.png
  images/nirt_fig08.png
  images/nirt_fig09.png
  images/nirt_fig10.png
  images/nirt_fig11.png
  images/nirt_fig12.png
  images/nirt_fig13.png
  images/nirt_fig14.png
  images/nirt_fig15.png
  images/nirt_fig16.png
  images/nirt_fig17.png
  images/nirt_fig18.png
  images/nirt_fig19.png
  images/nirt_fig20.png
  images/nirt_fig21.png
  images/nirt_fig22.png
  images/nirt_fig23.png
  images/nirt_fig24.png
  images/oed_fig01.png
  images/oed_fig02.png
  images/oed_fig03.png
  images/oed_fig04.png
  images/oed_fig05.png
  images/oed_fig06.png
  images/oed_fig07.png
  images/oed_fig08.png
  images/oed_fig09.png
  images/oed_fig10.png
  images/oed_fig11.png
  images/oed_fig12.png
  images/oed_fig13.png
  images/pipes_fig01.png
  images/pipes_fig02.png
  images/pipes_fig03.png
  images/pipes_fig04.png
  images/pipes_fig05.png
  images/pipes_fig06.png
  images/pipes_fig07.png
  images/pipes_fig08.png
  images/pipes_fig09.png
  images/pipes_fig10.png
  images/pipes_fig11.png
  images/pipes_fig12.png
  images/pipes_fig13.png
  images/projection_shader_fig01.png
  images/projection_shader_fig02.png
  images/projection_shader_fig03.png
  images/projection_shader_fig04.png
  images/projection_shader_fig05.png
  images/projection_shader_fig06.png
  images/projection_shader_fig07.png
  images/tire_fig01.png
  images/tire_fig02.png
  images/tire_fig03.png
  images/tire_fig04.png
  images/tire_fig05.png
  images/tire_fig06.png
  images/tire_fig07.png
  images/tire_fig08.png
  images/tire_fig09.png
  images/tire_fig10.png
  images/tire_fig11.png
  images/tire_fig12.png
  images/tire_fig13.png
  images/tire_fig14.png
  )
ADD_DOC(docbook_articles_EN_IMAGES html/articles/en/images)

DOCBOOK_TO_HTML(article docbook_articles_EN html/articles/en docbook_articles_EN_IMAGES_cp)

if(BRLCAD_EXTRADOCS_PDF)
  DOCBOOK_TO_PDF(article docbook_articles_EN pdf/articles/en docbook_articles_EN_IMAGES_cp)
endif(BRLCAD_EXTRADOCS_PDF)

CMAKEFILES(images/tire_fig12.svg images/tire_fig13.svg)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
