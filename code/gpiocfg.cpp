
#include "gpiodef.h"

void assert_failed(uint8_t* file, uint32_t line) {
  std::cerr << "Wrong parameters value: file " << file << " on line " << line << std::endl;
}

#define TYPEOUTPUT    0
#define TYPEINPUT     1

void GPIOCfg(std::string name, std::string GPIOx, std::vector<GPIO_InitTypeDef> GPIOcfg
    , bool splitpull, bool splitio
    , std::ofstream& cheader, std::ofstream& ccode) {

  uint32_t registeroffset = 0;

  uint32_t confighigh[2] = {0x00, 0x00};
  uint32_t configlow[2] = {0x00, 0x00};
  uint32_t *config;


  uint32_t maskhigh[2] = {0x00, 0x00};
  uint32_t masklow[2] = {0x00, 0x00};
  uint32_t *mask;

  uint32_t pullhighsrr = 0x00;
  uint32_t pulllowsrr = 0x00;
  uint32_t *pullsrr;

  uint32_t pullhighrr = 0x00;
  uint32_t pulllowrr = 0x00;
  uint32_t *pullrr;

  uint32_t ioposition;
  uint32_t iocurrent = 0x00;

  cheader << "void " << name << "(void);" << std::endl;
  ccode << "void " << name << "(void) {" << std::endl;

  for(auto const& e : GPIOcfg) {
    for (uint32_t position = 0; position < GPIO_NUMBER; position++) {
      ioposition = ((uint32_t) 0x01) << position;
      iocurrent = (uint32_t) (e.Pin) & ioposition;
      if (iocurrent == ioposition) {
        if (iocurrent < GPIO_PIN_8) {
          registeroffset = (position << 2);
          config = configlow;
          mask = masklow;
          pullsrr = &pulllowsrr;
          pullrr = &pulllowrr;
        } else {
          registeroffset = ((position - 8) << 2);
          config = confighigh;
          mask = maskhigh;
          pullsrr = &pullhighsrr;
          pullrr = &pullhighrr;
        }
        switch (e.Mode) {
          case GPIO_MODE_OUTPUT_PP:
            assert_param(IS_GPIO_SPEED(e.Speed));
            config[TYPEOUTPUT] |= (e.Speed + GPIO_CR_CNF_GP_OUTPUT_PP) << registeroffset;
            mask[TYPEOUTPUT] |= ((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << registeroffset);
            break;
          case GPIO_MODE_OUTPUT_OD:
            assert_param(IS_GPIO_SPEED(e.Speed));
            config[TYPEOUTPUT] |= (e.Speed + GPIO_CR_CNF_GP_OUTPUT_OD) << registeroffset;
            mask[TYPEOUTPUT] |= ((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << registeroffset);
            break;
          case GPIO_MODE_AF_PP:
            assert_param(IS_GPIO_SPEED(e.Speed));
            config[TYPEOUTPUT] |= (e.Speed + GPIO_CR_CNF_AF_OUTPUT_PP) << registeroffset;
            mask[TYPEOUTPUT] |= ((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << registeroffset);
            break;
          case GPIO_MODE_AF_OD:
            assert_param(IS_GPIO_SPEED(e.Speed));
            config[TYPEOUTPUT] |= (e.Speed + GPIO_CR_CNF_AF_OUTPUT_OD) << registeroffset;
            mask[TYPEOUTPUT] |= ((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << registeroffset);
            break;
          case GPIO_MODE_INPUT:
          case GPIO_MODE_IT_RISING:
          case GPIO_MODE_IT_FALLING:
          case GPIO_MODE_IT_RISING_FALLING:
          case GPIO_MODE_EVT_RISING:
          case GPIO_MODE_EVT_FALLING:
          case GPIO_MODE_EVT_RISING_FALLING:
            assert_param(IS_GPIO_PULL(e.Pull));
            mask[TYPEINPUT] |= ((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << registeroffset);
            if(e.Pull == GPIO_NOPULL)
            {
              config[TYPEINPUT] |= (GPIO_CR_MODE_INPUT + GPIO_CR_CNF_INPUT_FLOATING) << registeroffset;
            }
            else if(e.Pull == GPIO_PULLUP)
            {
              config[TYPEINPUT] |= (GPIO_CR_MODE_INPUT + GPIO_CR_CNF_INPUT_PU_PD) << registeroffset;
              *pullsrr |= ioposition;
            }
            else
            {
              config[TYPEINPUT] |= (GPIO_CR_MODE_INPUT + GPIO_CR_CNF_INPUT_PU_PD) << registeroffset;
              *pullrr |= ioposition;
            }
            break;
          case GPIO_MODE_ANALOG:
            config[TYPEINPUT] |= (GPIO_CR_MODE_INPUT + GPIO_CR_CNF_ANALOG) << registeroffset;
            mask[TYPEINPUT] |= ((GPIO_CRL_MODE0 | GPIO_CRL_CNF0) << registeroffset);
            break;
        }
      }
    }
  }
  if(splitio) {
    if(masklow[TYPEINPUT]) {
      ccode << "  /* LOW INPUT */" << std::endl;
      ccode << "  MODIFY_REG((" << GPIOx << "->CRL" << ")," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(masklow[TYPEINPUT]) << "," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(configlow[TYPEINPUT]) << std::endl;
      ccode << "  );" << std::endl;
      if(splitpull | ((pullhighrr | pullhighsrr) == 0x00) ) {
        if(pulllowrr) {
          ccode << "  " << GPIOx << "->BRR = " << pulllowrr << ";" << std::endl;
        }
        if(pulllowsrr) {
          ccode << "  " << GPIOx << "->BSRR = " << pulllowsrr << ";" << std::endl;
        }
      }
    }
    if(maskhigh[TYPEINPUT]) {
      ccode << "  /* HIGH INPUT */" << std::endl;
      ccode << "  MODIFY_REG((" << GPIOx << "->CRH" << ")," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(maskhigh[TYPEINPUT]) << "," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(confighigh[TYPEINPUT]) << std::endl;
      ccode << "  );" << std::endl;
      if (splitpull) {
        if(pullhighrr) {
          ccode << "  " << GPIOx << "->BRR = " << pullhighrr << ";" << std::endl;
        }
        if(pullhighsrr) {
          ccode << "  " << GPIOx << "->BRR = " << pullhighrr << ";" << std::endl;
        }
      } else {
        if(pullhighrr) {
          ccode << "  " << GPIOx << "->BRR = " << (pullhighrr | pulllowrr) << ";" << std::endl;
        }
        if(pullhighsrr) {
          ccode << "  " << GPIOx << "->BSRR = " << (pullhighsrr | pulllowsrr) << ";" << std::endl;
        }
      }
    }
    if(masklow[TYPEOUTPUT]) {
      ccode << "  /* LOW OUTPUT */" << std::endl;
      ccode << "  MODIFY_REG((" << GPIOx << "->CRL" << ")," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(masklow[TYPEOUTPUT]) << "," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(configlow[TYPEOUTPUT]) << std::endl;
      ccode << "  );" << std::endl;
    }
    if(maskhigh[TYPEOUTPUT]) {
      ccode << "  /* HIGH OUTPUT */" << std::endl;
      ccode << "  MODIFY_REG((" << GPIOx << "->CRH" << ")," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(maskhigh[TYPEOUTPUT]) << "," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(confighigh[TYPEOUTPUT]) << std::endl;
      ccode << "  );" << std::endl;
    }
  } else {
    if(masklow[TYPEINPUT] | masklow[TYPEOUTPUT]) {
      ccode << "  /* LOW */" << std::endl;
      ccode << "  MODIFY_REG((" << GPIOx << "->CRL" << ")," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(masklow[TYPEOUTPUT] | masklow[TYPEINPUT]) << "," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(configlow[TYPEOUTPUT] | configlow[TYPEINPUT]) << std::endl;
      ccode << "  );" << std::endl;
      if(splitpull | ((pullhighrr | pullhighsrr) == 0x00) ) {
        if(pulllowrr) {
          ccode << "  " << GPIOx << "->BRR = " << pulllowrr << ";" << std::endl;
        }
        if(pulllowsrr) {
            ccode << "  " << GPIOx << "->BSRR = " << pulllowsrr << ";" << std::endl;
        }
      }
    }
    if(maskhigh[TYPEINPUT] | maskhigh[TYPEOUTPUT]) {
      ccode << "  /* HIGH */" << std::endl;
      ccode << "  MODIFY_REG((" << GPIOx << "->CRH" << ")," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(maskhigh[TYPEOUTPUT] | maskhigh[TYPEINPUT]) << "," << std::endl;
      ccode << "    (uint32_t)0b" << std::bitset<32>(confighigh[TYPEOUTPUT] | confighigh[TYPEINPUT]) << std::endl;
      ccode << "  );" << std::endl;
      if (splitpull) {
        if(pullhighrr) {
          ccode << "  " << GPIOx << "->BRR = " << pullhighrr << ";" << std::endl;
        }
        if(pullhighsrr) {
          ccode << "  " << GPIOx << "->BRR = " << pullhighrr << ";" << std::endl;
        }
      } else {
        if(pullhighrr) {
          ccode << "  " << GPIOx << "->BRR = " << (pullhighrr | pulllowrr) << ";" << std::endl;
        }
        if(pullhighsrr) {
          ccode << "  " << GPIOx << "->BSRR = " << (pullhighsrr | pulllowsrr) << ";" << std::endl;
        }
      }
    }
  }
  ccode << "};" << std::endl;
}

int main(int argc, char* argv[]) {
  std::ofstream cheader;
  std::ofstream ccode;
  if ( argc != 3 ) {
    std::cerr << "usage: " << argv[0] << " <header dir path> <c dir path>" << std::endl;
    return 1;
  }
  cheader.open(std::string(argv[1]) + "/GPIO.h");
  ccode.open(std::string(argv[2]) + "/GPIO.c");

  cheader << "#include \"gpio.h\"" << std::endl << std::endl;
  ccode << "#include \"GPIO.h\"" << std::endl << std::endl;

  for(auto const& cfg : GPIOSetup) {
    GPIOCfg(cfg.func, cfg.GPIOx, cfg.GPIOcfg
        , cfg.splitpull, cfg.splitio
        , cheader, ccode);
  }
  cheader.close();
  ccode.close();
  return 0;
}


