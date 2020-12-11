#include "Core.h"
#include "CompilerData.h"

namespace Q2Compilers
{

	void to_json(json& j, const CData& p)
	{
		j = json{
			{ "qbsp_glView"      , p.qbsp_glView       },
			{ "qbsp_verbose"     , p.qbsp_verbose      },
			{ "qbsp_noweld"      , p.qbsp_noweld       },
			{ "qbsp_nodetail"    , p.qbsp_nodetail     },
			{ "qbsp_nowater"     , p.qbsp_nowater      },
			{ "qbsp_leaktest"    , p.qbsp_leaktest     },

			{ "qvis_threads"     , p.qvis_threads      },
			{ "qvis_fastvis"     , p.qvis_fastvis      },
			{ "qvis_qvis_verbose", p.qvis_qvis_verbose },
			{ "qvis_nosort"      , p.qvis_nosort       },
			{ "qvis_cullerror"   , p.qvis_cullerror    },

			{ "argh_dump"        , p.argh_dump         },
			{ "argh_bounce"      , p.argh_bounce       },
			{ "argh_verbose"     , p.argh_verbose      },
			{ "argh_extra"       , p.argh_extra        },
			{ "argh_update"      , p.argh_update       },
			{ "argh_threads"     , p.argh_threads      },
			{ "argh_chop"        , p.argh_chop         },
			//{ "argh_chopsky"     , p.argh_chopsky      },
			//{ "argh_chopwarp"    , p.argh_chopwarp     },
			//{ "argh_choplight"   , p.argh_choplight    },
			//{ "argh_chopcurve"   , p.argh_chopcurve    },
			{ "argh_scale"       , p.argh_scale        },
			{ "argh_direct"      , p.argh_direct       },
			{ "argh_entity"      , p.argh_entity       },
			{ "argh_texscale"    , p.argh_texscale     },
			{ "argh_glView"      , p.argh_glView       },
			{ "argh_nopvs"       , p.argh_nopvs        },
			{ "argh_nocolor"     , p.argh_nocolor      },
			{ "argh_saturation"  , p.argh_saturation   },
			{ "argh_ambient"     , p.argh_ambient      },
			{ "argh_minlight"    , p.argh_minlight     },
			{ "argh_maxlight"    , p.argh_maxlight     },
			{ "argh_gamma"       , p.argh_gamma        },
			{ "argh_stylemin"    , p.argh_stylemin     },
			{ "argh_radmin"      , p.argh_radmin       },
			{ "argh_nobmodlight" , p.argh_nobmodlight  },
			{ "argh_nobouncefix" , p.argh_nobouncefix  },
			{ "argh_nobrightsurf", p.argh_nobrightsurf },
			{ "argh_nostopbleed" , p.argh_nostopbleed  },
			{ "argh_nocurve"     , p.argh_nocurve      },
			{ "argh_nosplotchfix", p.argh_nosplotchfix },
			{ "argh_noradorigin" , p.argh_noradorigin  },
			{ "argh_lightwarp"   , p.argh_lightwarp    },

			{ "enable_qvis"      , p.enable_qvis       },
			{ "enable_qrad"      , p.enable_qrad       },
			{ "enable_copy"      , p.enable_copy       },
			{ "enable_exec"      , p.enable_exec       },

			{ "q2_directory"     , p.q2_directory.c_str()},
			{ "q2_modname"       , p.q2_modname.c_str()},
			{ "q2_executable"    , p.q2_executable.c_str()},
			{ "q2_args"          , p.q2_args.c_str()},
		};
	}

	void from_json(const json& j, CData& p)
	{
		j.at("qbsp_glView"      ).get_to(p.qbsp_glView);
		j.at("qbsp_verbose"     ).get_to(p.qbsp_verbose);
		j.at("qbsp_noweld"      ).get_to(p.qbsp_noweld);
		j.at("qbsp_nodetail"    ).get_to(p.qbsp_nodetail);
		j.at("qbsp_nowater"     ).get_to(p.qbsp_nowater);
		j.at("qbsp_leaktest"    ).get_to(p.qbsp_leaktest);
		j.at("qvis_threads"     ).get_to(p.qvis_threads);
		j.at("qvis_fastvis"     ).get_to(p.qvis_fastvis);
		j.at("qvis_qvis_verbose").get_to(p.qvis_qvis_verbose);
		j.at("qvis_nosort"      ).get_to(p.qvis_nosort);
		j.at("qvis_cullerror"   ).get_to(p.qvis_cullerror);
		j.at("argh_dump"        ).get_to(p.argh_dump);
		j.at("argh_bounce"      ).get_to(p.argh_bounce);
		j.at("argh_verbose"     ).get_to(p.argh_verbose);
		j.at("argh_extra"       ).get_to(p.argh_extra);
		j.at("argh_update"      ).get_to(p.argh_update);
		j.at("argh_threads"     ).get_to(p.argh_threads);
		j.at("argh_chop"        ).get_to(p.argh_chop);
		//j.at("argh_chopsky"     ).get_to(p.argh_chopsky);
		//j.at("argh_chopwarp"    ).get_to(p.argh_chopwarp);
		//j.at("argh_choplight"   ).get_to(p.argh_choplight);
		//j.at("argh_chopcurve"   ).get_to(p.argh_chopcurve);
		j.at("argh_scale"       ).get_to(p.argh_scale);
		j.at("argh_direct"      ).get_to(p.argh_direct);
		j.at("argh_entity"      ).get_to(p.argh_entity);
		j.at("argh_texscale"    ).get_to(p.argh_texscale);
		j.at("argh_glView"      ).get_to(p.argh_glView);
		j.at("argh_nopvs"       ).get_to(p.argh_nopvs);
		j.at("argh_nocolor"     ).get_to(p.argh_nocolor);
		j.at("argh_saturation"  ).get_to(p.argh_saturation);
		j.at("argh_ambient"     ).get_to(p.argh_ambient);
		j.at("argh_minlight"    ).get_to(p.argh_minlight);
		j.at("argh_maxlight"    ).get_to(p.argh_maxlight);
		j.at("argh_gamma"       ).get_to(p.argh_gamma);
		j.at("argh_stylemin"    ).get_to(p.argh_stylemin);
		j.at("argh_radmin"      ).get_to(p.argh_radmin);
		j.at("argh_nobmodlight" ).get_to(p.argh_nobmodlight);
		j.at("argh_nobouncefix" ).get_to(p.argh_nobouncefix);
		j.at("argh_nobrightsurf").get_to(p.argh_nobrightsurf);
		j.at("argh_nostopbleed" ).get_to(p.argh_nostopbleed);
		j.at("argh_nocurve"     ).get_to(p.argh_nocurve);
		j.at("argh_nosplotchfix").get_to(p.argh_nosplotchfix);
		j.at("argh_noradorigin" ).get_to(p.argh_noradorigin);
		j.at("argh_lightwarp"   ).get_to(p.argh_lightwarp);
		j.at("enable_qvis"      ).get_to(p.enable_qvis);
		j.at("enable_qrad"      ).get_to(p.enable_qrad);
		j.at("enable_copy"      ).get_to(p.enable_copy);
		j.at("enable_exec"      ).get_to(p.enable_exec);
		j.at("q2_directory"     ).get_to(p.q2_directory);
		j.at("q2_modname"       ).get_to(p.q2_modname);
		j.at("q2_executable"    ).get_to(p.q2_executable);
		j.at("q2_args"          ).get_to(p.q2_args);
	}

	bool CompilerData::LoadFromFile(const std::string& filename)
	{
		std::ifstream file("profiles/" + filename, std::ios_base::in);
		std::string content;

		if (file.good())
		{
			file >> j;
			Deserialize();

			LOG_TRACE("Loaded profile %s", filename.c_str());

			return true;
		}
		return false;
	}

	void CompilerData::SaveFile(const std::string& filename)
	{
		CreateDirectoryA("profiles", NULL);
		std::ofstream file("profiles/" + filename, std::ios_base::out | std::ios_base::trunc);

		if (file.good())
		{
			Serialize();

			file << std::setw(4) << j << std::endl;
			file.close();

			LOG_TRACE("Saved profile %s", filename.c_str());
		}
	}

	void CompilerData::Serialize()
	{
		j = data;
	}

	void CompilerData::Deserialize()
	{
		try
		{
			data = j;
		}
		catch (nlohmann::detail::exception e)
		{
			LOG_WARNING("Compiler data deserialization: %s", e.what());
		}
	}
}

