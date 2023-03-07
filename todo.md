# VCLib ToDo list

- Mesh:
  - Components:
    - [ ] add an enum COMPONENT_TYPE that allows to identify same class of components easily -- same usage of ELEMENT_TYPE for elements
      - [ ] then allow to write a generic enableSameOptionalComponentsOf member function in ElementContainer
  - Elements:
    - [ ] set properly elements concepts
    - [ ] set constructors for Edge element
    - [ ] set constructors for HalfEdge element
  - Containers:
    - [ ] set properly container concepts
  - Mesh:
    - [ ] manage clean() for all components of mesh, not only element containers
    - [ ] import dcel from mesh: update adjacent faces and wedges 
- Algorithms:
  - [ ] implement housdorff
  - [ ] update adjacent faces optimized for dcel
- I/O:
  - [ ] move appropriately load/save functions outside "vcl::io" namespace 
  - [ ] implement STL importer/exporter  
- External:
  - [ ] make render functions using bgfx
- Documentation:
  - [ ] Make documentation using only doxygen
  - [ ] Explain appropriately the structure of the library
  