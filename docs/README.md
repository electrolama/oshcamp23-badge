
# Head over to [https://electrolama.github.io/oshcamp23-badge/](https://electrolama.github.io/oshcamp23-badge/) for docs

## Notes

### Generating InteractiveHtmlBom for variants

See:
  - https://github.com/openscopeproject/InteractiveHtmlBom
  - https://github.com/Funkenjaeger/brd2json

Depending on which kit variant is used, there are some DNP parts. EAGLE source files have custom attributes set that works with the DNP option for InteractiveHtmlBom:

`python .\generate_interactive_bom.py "<path-to>/oshcamp23-badge/hardware/badge/Revision A2/oshcamp23-badge-RevA2.json" --extra-fields KIT_EASY --dnp-field KIT_EASY`

`python .\generate_interactive_bom.py "<path-to>/oshcamp23-badge/hardware/badge/Revision A2/oshcamp23-badge-RevA2.json" --extra-fields KIT_HARD --dnp-field KIT_HARD`